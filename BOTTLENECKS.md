# Performance Bottlenecks & Optimisation Guide

## Overview

microMC is an event-based Monte Carlo neutron transport code running on a single CPU core. Serpent 2 is orders of magnitude faster due to decades of optimisation, parallelism, and algorithmic refinements. However, there are several concrete bottlenecks in microMC that can be addressed for significant speedups without changing the overall architecture.

---

## Bottleneck 1: Redundant Cross Section Lookups

**Severity: Critical**

### Problem

`total_macro_xs()` performs a `binary_search` + linear interpolation for every reaction of every nuclide. This function is called **three separate times** per collision event at the same energy:

1. `event_advance` — to compute free-flight distance (`transport.cpp:11`)
2. `sample_reaction` — to sample which reaction occurs (`physics.cpp:50`)
3. `score_flux` — to compute the collision estimator `w / Sigma_t` (`transport.cpp:91`)

Each call to `total_macro_xs` loops over all nuclides and all their reactions. For a UO2+H2O material with 4 nuclides and ~30 total reactions, that's ~90 binary searches per collision instead of ~30.

### Fix

Cache `Sigma_t` on the `Neutron` struct so it is computed once and reused.

#### Step 1: Add a field to `Neutron` in `include/neutron.h`

```cpp
struct Neutron {
  double x{0.0};
  double y{0.0};
  double z{0.0};
  double E{0.0};
  double Omega_x{0.0};
  double Omega_y{0.0};
  double Omega_z{0.0};
  double w{1.0};
  double sigma_t{0.0};   // <-- cached total macroscopic XS
  bool alive{true};
  ReactionSample rxn;
};
```

#### Step 2: Add a new event function to compute and cache XS

In `transport.cpp`:

```cpp
void event_calc_xs(TransportState &state) {
  for (auto &n : state.current_bank) {
    if (!n.alive) continue;
    n.sigma_t = total_macro_xs(*state.material, *state.data, n.E);
  }
}
```

#### Step 3: Use cached value everywhere

In `event_advance`:
```cpp
void event_advance(TransportState &state) {
  for (auto &n : state.current_bank) {
    double d = -std::log(uniform(state.rng)) / n.sigma_t;
    n.x += d * n.Omega_x;
    n.y += d * n.Omega_y;
    n.z += d * n.Omega_z;
  }
}
```

In `sample_reaction` (physics.cpp), change line 50:
```cpp
double xi = uniform(rng) * total_macro_xs(mat, data, E);
```
to accept the cached value:
```cpp
ReactionSample sample_reaction(const Material &mat, const NuclearData &data,
                               double E, double sigma_t, RNG &rng) {
  double xi = uniform(rng) * sigma_t;
  // ... rest unchanged
}
```

In `score_flux`:
```cpp
void score_flux(TransportState &state) {
  for (const auto &neutron : state.current_bank) {
    if (!neutron.alive) continue;
    double score = neutron.w / neutron.sigma_t;
    state.tally_file.write(...);
  }
}
```

#### Step 4: Update the transport loop

```cpp
while (!state.current_bank.empty()) {
    event_calc_xs(state);         // compute XS once
    event_advance(state);          // uses cached sigma_t
    event_sample_reaction(state);  // uses cached sigma_t
    if (state.scoring_active) score_flux(state);  // uses cached sigma_t
    event_process_collision(state);
    event_compact_bank(state.current_bank);
}
```

**Expected speedup: ~2-3x** (eliminates ~2/3 of all cross section work).

---

## Bottleneck 2: Redundant Binary Searches Within `total_macro_xs` and `sample_reaction`

**Severity: High**

### Problem

Inside `total_macro_xs` and `sample_reaction`, `lookup_xs` is called for every reaction of a nuclide. Each call performs its own `binary_search` on the nuclide's energy grid:

```cpp
// material.cpp:30-31 — called for EVERY reaction r
for (int r = 0; r < nuc.n_reactions; ++r)
    sigma_t += N * lookup_xs(data, nuc, data.reactions[nuc.rxn_offset + r], E);
```

But all reactions of a nuclide share the same energy grid. So for U238 with ~20 reactions, the same `binary_search` on the same grid is repeated 20 times. The search result (grid index `i` and interpolation factor `t`) is identical every time.

### Fix

Factor the binary search out of `lookup_xs` so it happens once per nuclide.

#### Step 1: Add a fast XS lookup that takes a pre-computed grid index

In `physics.h`:
```cpp
// Find the grid index and interpolation factor for a nuclide at energy E
struct GridIndex {
  int i;     // grid bin index
  double t;  // interpolation fraction
};

GridIndex find_grid_index(const NuclearData &data, const NuclideDescriptor &nuc, double E);

// Fast XS lookup using pre-computed grid index (no binary search)
double lookup_xs_fast(const NuclearData &data, const ReactionDescriptor &rxn,
                      const GridIndex &gi);
```

#### Step 2: Implement in `physics.cpp`

```cpp
GridIndex find_grid_index(const NuclearData &data, const NuclideDescriptor &nuc, double E) {
  const double *E_grid = data.energy_grids + nuc.grid_offset;
  int n = nuc.grid_length;
  int i = binary_search(E_grid, n, E);
  if (i >= n - 1) i = n - 2;
  double t = (E - E_grid[i]) / (E_grid[i + 1] - E_grid[i]);
  return {i, t};
}

double lookup_xs_fast(const NuclearData &data, const ReactionDescriptor &rxn,
                      const GridIndex &gi) {
  int j = gi.i - rxn.threshold_idx;
  if (j < 0) return 0.0;
  if (j >= rxn.xs_length - 1) return data.xs_pool[rxn.xs_offset + rxn.xs_length - 1];
  double xs_lo = data.xs_pool[rxn.xs_offset + j];
  double xs_hi = data.xs_pool[rxn.xs_offset + j + 1];
  return xs_lo + gi.t * (xs_hi - xs_lo);
}
```

#### Step 3: Update `total_macro_xs` in `material.cpp`

```cpp
double total_macro_xs(const Material &mat, const NuclearData &data, double E) {
  double sigma_t = 0.0;
  for (int i = 0; i < mat.n_nuclides; ++i) {
    const auto &nuc = data.nuclides[mat.nuclide_ids[i]];
    GridIndex gi = find_grid_index(data, nuc, E);  // ONE search per nuclide
    for (int r = 0; r < nuc.n_reactions; ++r)
      sigma_t += mat.number_densities[i] *
                 lookup_xs_fast(data, data.reactions[nuc.rxn_offset + r], gi);
  }
  return sigma_t;
}
```

Apply the same pattern to `sample_reaction`.

**Expected speedup: ~1.5-2x** on top of Bottleneck 1 fix. For U238 with 20 reactions, this eliminates 19 out of 20 binary searches per nuclide.

---

## Bottleneck 3: RNG Overhead — `std::uniform_real_distribution` Reconstruction

**Severity: Medium**

### Problem

Every call to `uniform()` constructs a temporary `std::uniform_real_distribution<double>`:

```cpp
inline double uniform(RNG& rng) {
    return std::uniform_real_distribution<double>{0.0, 1.0}(rng);
}
```

While a good compiler may optimise away the constructor, this is not guaranteed. Each neutron history consumes ~10-20 random numbers per collision (advance, reaction sampling, scattering, direction rotation), so this is called millions of times.

### Fix

Replace with a direct conversion from the raw 64-bit integer output of `mt19937_64`. This is the standard trick used in production MC codes.

```cpp
inline double uniform(RNG& rng) {
    // Take upper 53 bits of the 64-bit output, map to [0, 1)
    return (rng() >> 11) * (1.0 / 9007199254740992.0);  // 1 / 2^53
}
```

This avoids all distribution object overhead and produces a uniformly distributed double in `[0, 1)` with full 53-bit mantissa precision.

**Expected speedup: ~5-15%** overall, since RNG is called in every hot path.

---

## Bottleneck 4: Unbuffered Binary I/O in `score_flux`

**Severity: Medium-High (scales with particle count)**

### Problem

`score_flux` performs two 8-byte `write()` calls per alive neutron per collision:

```cpp
state.tally_file.write(reinterpret_cast<const char *>(&neutron.E), sizeof(double));
state.tally_file.write(reinterpret_cast<const char *>(&score), sizeof(double));
```

With 1000 particles, ~50 collisions/history, 1000 active cycles, that is ~100 million `write()` calls. Even with OS buffering, the per-call overhead of `std::ofstream::write` (virtual dispatch, buffer management) adds up.

### Fix

Use an in-memory buffer and flush periodically.

#### Step 1: Add a tally buffer to `TransportState`

```cpp
struct TransportState {
  // ... existing fields ...
  std::vector<double> tally_buffer;
};
```

#### Step 2: Buffer writes in `score_flux`

```cpp
void score_flux(TransportState &state) {
  for (const auto &neutron : state.current_bank) {
    if (!neutron.alive) continue;
    double score = neutron.w / neutron.sigma_t;
    state.tally_buffer.push_back(neutron.E);
    state.tally_buffer.push_back(score);
  }
}
```

#### Step 3: Flush at end of each cycle (or when buffer is large)

After the inner `while` loop in `run_eigenvalue`:

```cpp
if (!state.tally_buffer.empty()) {
  state.tally_file.write(
      reinterpret_cast<const char *>(state.tally_buffer.data()),
      state.tally_buffer.size() * sizeof(double));
  state.tally_buffer.clear();
}
```

#### Step 4: Pre-allocate the buffer

Before the cycle loop:
```cpp
state.tally_buffer.reserve(n_particles * 100);  // ~50 collisions * 2 doubles
```

**Expected speedup: ~10-30%** for large runs where I/O becomes significant. Reduces millions of small writes to ~1000 bulk writes.

---

## Bottleneck 5: `event_compact_bank` Every Transport Step

**Severity: Low-Medium**

### Problem

`event_compact_bank` calls `std::partition` on the entire bank every transport step:

```cpp
void event_compact_bank(ParticleBank &bank) {
  auto it = std::partition(bank.begin(), bank.end(),
                           [](const Neutron &n) { return n.alive; });
  bank.erase(it, bank.end());
}
```

In early transport steps, very few neutrons have died (most collisions are scattering). Partitioning a 1000-element bank when only 5 neutrons died is wasteful — you're touching all 1000 elements to remove 5.

### Fix Option A: Compact less frequently

Only compact when the dead fraction exceeds a threshold:

```cpp
void event_compact_bank(ParticleBank &bank) {
  // Count dead neutrons
  int n_dead = 0;
  for (const auto &n : bank)
    if (!n.alive) ++n_dead;

  // Only compact if >25% are dead
  if (n_dead * 4 > static_cast<int>(bank.size())) {
    auto it = std::partition(bank.begin(), bank.end(),
                             [](const Neutron &n) { return n.alive; });
    bank.erase(it, bank.end());
  }
}
```

### Fix Option B: Skip dead neutrons in event functions instead

Add `if (!n.alive) continue;` at the top of `event_advance` and `event_sample_reaction`, and remove `event_compact_bank` from the inner loop entirely. Only compact once at the end of the cycle. This is simpler and avoids the partition entirely during transport.

**Expected speedup: ~5-10%** — modest because the bank is small, but becomes more meaningful at higher particle counts.

---

## Bottleneck 6: Direction Rotation Renormalisation

**Severity: Low**

### Problem

`rotate_dir` (physics.cpp:135-139) computes a full `sqrt` + three divisions to renormalise the direction vector at the end of every call:

```cpp
double norm = std::sqrt(Omega_x * Omega_x + Omega_y * Omega_y + Omega_z * Omega_z);
Omega_x /= norm;
Omega_y /= norm;
Omega_z /= norm;
```

With `-ffast-math` and correct formulas, the output should already be unit-length to machine precision. The renormalisation is defensive but adds a `sqrt` + 3 divisions per scatter.

### Fix

Remove the renormalisation and rely on the rotation formula being correct (which it is, given the clamp on `mu`). If paranoid, renormalise every N collisions instead of every one:

```cpp
// Only renormalise every 100 scatters (add a scatter counter to Neutron if desired)
// Or just remove entirely — the rotation formula preserves unit length.
```

**Expected speedup: ~2-5%** — one fewer `sqrt` per collision.

---

## Bottleneck 7: `std::cout` Formatting Every Cycle

**Severity: Low (but annoying)**

### Problem

Every cycle sets `std::fixed` and `std::setprecision(5)` and prints to stdout:

```cpp
std::cout << std::fixed << std::setprecision(5);
std::cout << "cycle " << std::setw(4) << c << "  k_eff = " << state.k_eff ...
```

Terminal I/O is slow. With 1050 cycles, this is minor, but if you ever run 10,000+ cycles it becomes noticeable.

### Fix

Print every Nth cycle (e.g., every 10th or 50th):

```cpp
if (c % 10 == 0 || c == n_inactive + n_active - 1) {
  // ... print
}
```

---

## Summary: Priority Order

| # | Bottleneck | Speedup | Effort |
|---|-----------|---------|--------|
| 1 | Cache Sigma_t (3x redundant lookup) | 2-3x | Low |
| 2 | Factor binary search per-nuclide | 1.5-2x | Low |
| 3 | RNG distribution overhead | 5-15% | Trivial |
| 4 | Buffer tally I/O | 10-30% | Low |
| 5 | Compact bank less often | 5-10% | Trivial |
| 6 | Remove direction renorm | 2-5% | Trivial |
| 7 | Reduce print frequency | ~1% | Trivial |

Fixing bottlenecks 1-4 together should give a **~4-6x overall speedup**. Combined with the compiler flag upgrade (`-O3 -march=native -ffast-math`), expect roughly an order of magnitude improvement over the original build.

Note: Serpent will still be faster due to multithreading, unionised energy grids, memory-mapped XS data, and 20+ years of micro-optimisations. But these changes bring microMC much closer for a single-threaded code.
