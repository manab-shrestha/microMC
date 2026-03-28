# Event-Based Transport Implementation Guide

This guide walks through implementing an event-based k-eigenvalue solver for an infinite homogeneous medium. Each section builds on the last — implement in order, building after each section.

---

## Phase 1: Update Neutron Struct

**File:** `include/neutron.h`

Add two fields to `Neutron`:

```cpp
bool alive{true};
ReactionSample rxn;  // filled by event 1, consumed by event 2
```

`alive` is needed because the event-based loop processes all particles per event. Dead particles (absorption, fission) must persist until the compaction event removes them. `rxn` stores the sampled reaction between event 1 and event 2 — this decoupling is what makes the design event-based.

You'll need to forward-include or include `physics.h` for `ReactionSample`. Watch the include order — `physics.h` includes `NuclearData.h` and `rng.h`, so `neutron.h` will pull those in. Alternatively, forward-declare `ReactionSample` and include `physics.h` only in the `.cpp` files.

**Build check:** `make` — nothing uses the new fields yet, so it should compile cleanly.

---

## Phase 2: Utility Sampling Functions

**Files:** `include/physics.h` (declarations), `source/physics.cpp` (implementations)

These are pure-math building blocks with no physics knowledge.

### 2a. `sample_tabulated_cdf`

```
double sample_tabulated_cdf(const double* x, const double* pdf,
                            const double* cdf, int n, RNG& rng);
```

This is the workhorse sampler used by every distribution. Algorithm:

1. Draw `xi = uniform(rng)`
2. Binary search `cdf` to find bin `i` where `cdf[i] <= xi < cdf[i+1]`
3. Linear interpolation within that bin: `x_out = x[i] + (x[i+1] - x[i]) * (xi - cdf[i]) / (cdf[i+1] - cdf[i])`

Edge cases: clamp `i` so you never read out of bounds. If `cdf[i+1] == cdf[i]` (zero-probability bin), return `x[i]`.

### 2b. `sample_isotropic_direction`

```
void sample_isotropic_direction(double& Ox, double& Oy, double& Oz, RNG& rng);
```

Sample a direction uniformly on the unit sphere:

1. `mu = 2 * uniform(rng) - 1` (cosine of polar angle, uniform on [-1,1])
2. `phi = 2 * pi * uniform(rng)`
3. `sin_theta = sqrt(1 - mu*mu)`
4. `Ox = sin_theta * cos(phi)`, `Oy = sin_theta * sin(phi)`, `Oz = mu`

### 2c. `rotate_direction`

```
void rotate_direction(double& Ox, double& Oy, double& Oz,
                      double mu, RNG& rng);
```

Given current direction `(Ox, Oy, Oz)` and scattering cosine `mu`, sample azimuthal angle `phi = 2*pi*uniform(rng)` and rotate. This is the standard formula from Lux & Koblinger or any MC textbook:

```
sin_theta = sqrt(1 - mu*mu)
cos_phi = cos(phi)
sin_phi = sin(phi)

if |Oz| < 0.9999:
    A = sqrt(1 - Oz*Oz)
    Ox_new = sin_theta * (Ox*Oz*cos_phi - Oy*sin_phi) / A + Ox*mu
    Oy_new = sin_theta * (Oy*Oz*cos_phi + Ox*sin_phi) / A + Oy*mu
    Oz_new = -sin_theta * cos_phi * A + Oz*mu
else:
    // Near poles, use simplified formula
    Ox_new = sin_theta * cos_phi
    Oy_new = sin_theta * sin_phi
    Oz_new = copysign(mu, Oz)
```

**Build check:** `make`. You can add a temporary test in `main.cpp` that calls these and prints results to verify ranges.

---

## Phase 3: Distribution Samplers

These use the pool data structures from `NuclearData.h` and the utility samplers from Phase 2.

### 3a. Stochastic Energy Interpolation (shared pattern)

Every distribution pool (angular, fission energy, Kalbach) uses the same two-level lookup:

1. A distribution `dist_id` has incident energies at `energies[energy_offsets[dist_id] .. energy_offsets[dist_id+1]-1]`
2. Binary search to find the two bounding energies `E_lo`, `E_hi` around incident energy `E`
3. **Stochastic interpolation:** pick either the lower or upper table with probability proportional to the interpolation fraction: `f = (E - E_lo) / (E_hi - E_lo)`, use upper if `uniform(rng) < f`, else lower
4. The selected energy index `j` has its tabulated data at `table_offsets[j] .. table_offsets[j+1]-1`

This gives you the offset and length into the `mu`/`pdf`/`cdf` (or `E_out`/`pdf`/`cdf`) arrays.

### 3b. `sample_angular_cosine`

```
double sample_angular_cosine(const AngularDistPool& pool, int dist_id,
                             double E, RNG& rng);
```

1. Stochastic energy interpolation (3a) to select an energy table
2. `sample_tabulated_cdf(mu, pdf, cdf, n_points, rng)` on the selected table
3. Return sampled `mu` (scattering cosine in CM frame)

### 3c. `sample_fission_energy`

```
double sample_fission_energy(const EnergyDistPool& pool, int dist_id,
                             double E, RNG& rng);
```

Same structure as 3b but sampling outgoing energy instead of cosine:

1. Stochastic energy interpolation to select an energy table
2. `sample_tabulated_cdf(E_out, pdf, cdf, n_points, rng)`
3. Return sampled `E_out`

### 3d. `sample_kalbach_mann`

```
struct KalbachResult { double E_out; double mu; };

KalbachResult sample_kalbach_mann(const KalbachMannDistPool& pool, int dist_id,
                                  double E, RNG& rng);
```

1. Stochastic energy interpolation to select an energy table
2. `sample_tabulated_cdf(E_out, pdf, cdf, n_points, rng)` to get outgoing energy
3. Interpolate `r` and `a` at the sampled `E_out` position (same bin index from the CDF sample)
4. Sample scattering cosine from the Kalbach-Mann formula:
   - Draw `xi = uniform(rng)`
   - `T = (2*xi - 1) * sinh(a)`
   - If `uniform(rng) > r`: `mu = log(T + sqrt(T*T + 1)) / a` (equiprobable in sinh distribution)
   - Else: `mu = log(xi * exp(a) + (1-xi) * exp(-a)) / a` (forward-peaked component)
   - Clamp `mu` to `[-1, 1]`

### 3e. `sample_nu_bar`

```
int sample_nu_bar(const FissionYieldPool& pool, int yield_id,
                  double E, RNG& rng);
```

1. Look up `FissionYieldDescriptor` for `yield_id` → offset, n_points
2. Binary search `pool.energy[offset..offset+n_points-1]` for `E`
3. Linearly interpolate `pool.nu_bar` at that point → `nu_bar`
4. Stochastic rounding: `nu = floor(nu_bar)`, if `uniform(rng) < (nu_bar - nu)` then `nu += 1`
5. Return `nu`

**Build check:** `make`. Again, temporary prints in main to verify output ranges.

---

## Phase 4: Collision Kinematics

These modify the neutron's energy and direction in-place (and may create secondaries).

### 4a. `elastic_scatter`

```
void elastic_scatter(Neutron& n, const NuclideDescriptor& nuc,
                     const NuclearData& data, RNG& rng);
```

1. Get the elastic reaction (first reaction for this nuclide, type == ELASTIC)
2. If `rxn.dist_id >= 0`: `mu_cm = sample_angular_cosine(data.angular, rxn.dist_id, n.E, rng)`
   Else: `mu_cm = 2*uniform(rng) - 1` (isotropic in CM)
3. `A = nuc.A` (atomic weight ratio)
4. Outgoing energy (two-body, CM→lab):
   `E_out = n.E * (A*A + 2*A*mu_cm + 1) / ((A+1)*(A+1))`
5. Lab-frame scattering cosine:
   `mu_lab = (A*mu_cm + 1) / sqrt(A*A + 2*A*mu_cm + 1)`
6. `rotate_direction(n.Omega_x, n.Omega_y, n.Omega_z, mu_lab, rng)`
7. `n.E = E_out`

### 4b. `inelastic_scatter`

```
void inelastic_scatter(Neutron& n, const NuclideDescriptor& nuc,
                       const ReactionDescriptor& rxn,
                       const NuclearData& data, RNG& rng);
```

**Discrete inelastic** (`rxn.type == DISCRETE_INELASTIC`):

1. `mu_cm = sample_angular_cosine(data.angular, rxn.dist_id, n.E, rng)`
2. Two-body kinematics with Q-value:
   - `A = nuc.A`
   - `E_cm = n.E * A / (A + 1)` (CM energy)
   - `E_cm_out = E_cm + rxn.Q_value` (energy available after excitation)
   - If `E_cm_out <= 0`: absorb (set `n.alive = false`, return)
   - `E_out = E_cm_out * (A*A + 2*A*mu_cm + 1) / ((A+1)*(A+1))` + correction terms
   - More precisely, use the full two-body formula:
     `E_out = n.E / (A+1)^2 * (cos_theta_cm + sqrt(A^2 - (1-cos_theta_cm^2)))^2`
     where you account for Q by adjusting A effectively. Refer to the ENDF manual (Appendix H) or OpenMC source for the exact formula.
3. Lab cosine and direction rotation as in elastic
4. `n.E = E_out`

**Continuum inelastic** (`rxn.type == CONTINUUM_INELASTIC`):

1. `auto [E_out, mu] = sample_kalbach_mann(data.kalbach, rxn.dist_id, n.E, rng)`
2. `rotate_direction(n.Omega_x, n.Omega_y, n.Omega_z, mu, rng)`
3. `n.E = E_out`

### 4c. `fission_event`

```
void fission_event(Neutron& n, const NuclideDescriptor& nuc,
                   const ReactionDescriptor& rxn,
                   const NuclearData& data,
                   ParticleBank& fission_bank,
                   double k_eff, RNG& rng);
```

1. `int nu = sample_nu_bar(data.fission_yields, rxn.yield_id, n.E, rng)`
2. For each of the `nu` secondary neutrons:
   - Create a `Neutron` at same position `(n.x, n.y, n.z)`
   - `sample_isotropic_direction(...)` for its direction
   - `E_out = sample_fission_energy(data.fission_energy, rxn.dist_id, n.E, rng)`
   - `weight = n.w / k_eff` (population control — prevents exponential growth/decay)
   - Push into `fission_bank`
3. `n.alive = false`

### 4d. `n_xn_event`

```
void n_xn_event(Neutron& n, const ReactionDescriptor& rxn,
                const NuclearData& data,
                ParticleBank& source_bank, RNG& rng);
```

1. `auto [E_out, mu] = sample_kalbach_mann(data.kalbach, rxn.dist_id, n.E, rng)`
2. `int n_secondary = rxn.multiplicity - 1` (1 for n,2n; 2 for n,3n — the original neutron continues as one of the products)
3. For each secondary:
   - Create neutron at same position, same weight
   - `sample_isotropic_direction(...)` (or sample another Kalbach if you want correlated — isotropic is a reasonable approximation)
   - Sample another energy from the same Kalbach distribution
   - Push into `source_bank`
4. Update original: `n.E = E_out`, `rotate_direction(..., mu, rng)`

**Build check:** `make`

---

## Phase 5: Transport Events

**New files:** `include/transport.h`, `source/transport.cpp`

### TransportState

```cpp
struct TransportState {
    ParticleBank source_bank;
    ParticleBank fission_bank;
    const Material* material;
    const NuclearData* data;
    RNG rng;
    double k_eff;
    int cycle;
};
```

### Event 1: `event_sample_reaction`

```cpp
void event_sample_reaction(TransportState& state);
```

Loop over `source_bank`. For each particle where `alive == true`:
```
particle.rxn = sample_reaction(*state.material, *state.data, particle.E, state.rng);
```

### Event 2: `event_process_collision`

```cpp
void event_process_collision(TransportState& state);
```

Loop over `source_bank`. For each alive particle, switch on `particle.rxn.type`:

| RxnType | Action |
|---------|--------|
| `ELASTIC` | `elastic_scatter(p, nuc, data, rng)` |
| `DISCRETE_INELASTIC` | `inelastic_scatter(p, nuc, rxn, data, rng)` |
| `CONTINUUM_INELASTIC` | `inelastic_scatter(p, nuc, rxn, data, rng)` |
| `FISSION` | `fission_event(p, nuc, rxn, data, fission_bank, k_eff, rng)` |
| `ABSORPTION` | `p.alive = false` |
| `N2N` | `n_xn_event(p, rxn, data, source_bank, rng)` |
| `N3N` | `n_xn_event(p, rxn, data, source_bank, rng)` |

Access `nuc` and `rxn` via `data.nuclides[p.rxn.nuclide_id]` and `data.reactions[p.rxn.rxn_id]`.

**Important:** when iterating with an index-based for loop, `n_xn_event` may push new particles onto the back of `source_bank`. These new particles have `alive = true` and will be processed next iteration of the outer while loop. Use an index loop with a captured size: `int n = source_bank.size(); for (int i = 0; i < n; ++i)`.

### Event 3: `event_compact_bank`

```cpp
void event_compact_bank(ParticleBank& bank);
```

```cpp
auto it = std::partition(bank.begin(), bank.end(),
                         [](const Neutron& n) { return n.alive; });
bank.erase(it, bank.end());
```

---

## Phase 6: k-Eigenvalue Driver

**File:** `source/transport.cpp`

```cpp
void run_eigenvalue(const Material& mat, const NuclearData& data,
                    int n_particles, int n_inactive, int n_active,
                    uint64_t seed);
```

### Initialization

1. Seed `RNG rng(seed)`
2. Create `source_bank` with `n_particles` neutrons:
   - Position: `(0, 0, 0)` (infinite medium — doesn't matter)
   - Direction: `sample_isotropic_direction`
   - Energy: `uniform(rng) * 5e6` (flat 1 eV–5 MeV — will thermalize after a few inactive cycles). A Watt spectrum would be more physical but flat is fine for getting started.
   - Weight: `1.0`
   - Alive: `true`
3. `k_eff = 1.0`

### Cycle Loop

```
for c in 0 .. (n_inactive + n_active - 1):
    int start_size = source_bank.size()

    while source_bank is not empty:
        event_sample_reaction(state)
        event_process_collision(state)
        event_compact_bank(state.source_bank)

    k_eff = k_eff * (double)fission_bank.size() / (double)start_size

    swap(source_bank, fission_bank)
    fission_bank.clear()

    // Population control: resample to exactly n_particles
    // Simple approach: normalize weights so sum = n_particles
    double total_w = 0;
    for (auto& p : source_bank) total_w += p.w;
    double factor = (double)n_particles / total_w;
    for (auto& p : source_bank) p.w *= factor;

    // If bank is too large or too small, randomly sample/duplicate to target size
    // (skip this for v1 — weight normalization is sufficient)

    if (c >= n_inactive):
        accumulate k_eff for running mean and variance

    print: cycle c, k_eff, bank size
```

### Output

After all cycles, print:
- `k_eff = mean +/- std_dev`
- `std_dev = sqrt(variance / n_active)`

---

## Phase 7: Wire Up Main

**File:** `source/main.cpp`

Replace the commented-out test code:

```cpp
int main() {
    // ... existing load + material setup ...

    run_eigenvalue(fuel, data, 1000, 50, 100, 42);
    return 0;
}
```

Start with small particle counts (1000) for debugging. Increase to 10000+ once things converge.

---

## Debugging Checklist

- [ ] **Fission bank never empty?** If it's empty after a cycle, particles are being absorbed too fast. Check that fission XS is being picked up and `sample_nu_bar` returns > 0.
- [ ] **k_eff diverges or goes to 0?** Check that fission weight uses `w / k_eff`. Without this, the fission bank grows/shrinks exponentially.
- [ ] **Negative or zero energies?** Check inelastic kinematics — `E_cm_out` can go negative if `E_cm < |Q|`. Handle by absorbing.
- [ ] **Infinite loop in transport?** If no reaction kills particles, the while loop never terminates. Make sure absorption and fission set `alive = false`.
- [ ] **NaN directions?** Check `rotate_direction` near-pole guard (`|Oz| > 0.9999`).
- [ ] **k_eff ~ 1.0–1.4?** For UO2+H2O with these number densities, expect roughly this range. If wildly different, check number densities and ZAID resolution.

---

## Reference Formulas

### Two-body elastic kinematics (CM frame)
```
E_out = E_in * (A^2 + 2*A*mu_cm + 1) / (A + 1)^2
mu_lab = (A*mu_cm + 1) / sqrt(A^2 + 2*A*mu_cm + 1)
```

### Kalbach-Mann angular sampling
```
Given r, a at the sampled E_out:
xi1, xi2 = uniform(rng)

if xi1 > r:
    T = (2*xi2 - 1) * sinh(a)
    mu = ln(T + sqrt(T^2 + 1)) / a
else:
    mu = ln(xi2 * exp(a) + (1 - xi2) * exp(-a)) / a
```

### Stochastic rounding for nu-bar
```
nu_bar = interpolated value
nu = floor(nu_bar)
if uniform(rng) < (nu_bar - nu):
    nu += 1
```
