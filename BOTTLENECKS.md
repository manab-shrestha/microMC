# Performance Bottlenecks & Implementation Guide

Analysis of the MicroMC eigenvalue Monte Carlo neutron transport code. Bottlenecks
are ordered by estimated performance impact (highest first). Each section includes:
what the problem is, where it occurs, why it matters, and concrete implementation
steps.

---

## 1. Redundant Cross-Section Computation (3x per collision)

**Impact: ~2-3x speedup achievable**

### Problem

Every neutron collision triggers up to **three independent calls** to
`total_macro_xs()`, each of which loops over all nuclides and all reactions
with a binary search per reaction:

| Call site | Location | Purpose |
|---|---|---|
| `event_advance` | `transport.cpp:11` | Distance sampling: `sigma_t` for exponential |
| `sample_reaction` | `physics.cpp:50` | Reaction sampling: `uniform(rng) * total_macro_xs(...)` |
| `score_flux` | `transport.cpp:91` | Tally: `neutron.w / sigma_t` |

`sample_reaction` then **re-walks** every nuclide/reaction to accumulate the
CDF, calling `lookup_xs` again for each reaction. So the total XS lookup count
per collision is:

```
event_advance:        N_nuclides * N_rxns_each  (binary_search + interp)
sample_reaction:      N_nuclides * N_rxns_each  (for total)
                    + N_nuclides * N_rxns_each  (for CDF walk, partial avg)
score_flux:           N_nuclides * N_rxns_each  (if scoring)
```

For a 4-nuclide UO2+H2O material with ~8-12 reactions per nuclide, that's
~100-150 binary searches per collision when only ~30-40 are needed.

### Fix

#### Step 1: Add a field to `Neutron` in `include/neutron.h`

```cpp
struct Neutron {
  // ... existing fields ...
  double sigma_t{0.0};   // cached total macroscopic XS at current E
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

#### Step 3: Use cached value in `event_advance`

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

#### Step 4: Pass cached sigma_t into `sample_reaction`

Change the signature:

```cpp
ReactionSample sample_reaction(const Material &mat, const NuclearData &data,
                               double E, double sigma_t, RNG &rng);
```

Inside, replace `uniform(rng) * total_macro_xs(mat, data, E)` with
`uniform(rng) * sigma_t`.

#### Step 5: Reuse in `score_flux`

```cpp
double score = neutron.w / neutron.sigma_t;
```

#### Step 6: Update the transport loop order

```cpp
while (!state.current_bank.empty()) {
    event_calc_xs(state);          // compute XS once
    event_advance(state);          // uses cached sigma_t
    event_sample_reaction(state);  // uses cached sigma_t
    if (state.scoring_active) score_flux(state);
    event_process_collision(state);
    event_compact_bank(state.current_bank);
}
```

---

## 2. Redundant Binary Search Per Reaction (Same Energy Grid)

**Impact: ~1.5-2x on top of fix #1**

### Problem

All reactions of a given nuclide share the **same energy grid**
(`data.energy_grids + nuc.grid_offset`). Yet `lookup_xs()` at `physics.cpp:31`
performs an independent `binary_search` for every reaction:

```cpp
double lookup_xs(...) {
  const double *E_grid = data.energy_grids + nuc.grid_offset;
  int n = nuc.grid_length;
  int i = binary_search(E_grid, n, E);  // same result for all rxns of this nuclide
  ...
}
```

In `total_macro_xs` (`material.cpp:126`), the inner loop calls `lookup_xs` for
each reaction `r` of nuclide `i`. The binary search on a grid of ~1000 points
costs ~10 comparisons, done needlessly N_rxns times per nuclide.

For U238 with ~20 reactions, 19 out of 20 binary searches are wasted.

### Fix

#### Step 1: Create a `GridIndex` struct and finder function

In `physics.h`:

```cpp
struct GridIndex {
  int i;     // grid bin index
  double t;  // interpolation fraction within bin
};

GridIndex find_grid_index(const NuclearData &data,
                          const NuclideDescriptor &nuc, double E);

double lookup_xs_fast(const NuclearData &data, const ReactionDescriptor &rxn,
                      const GridIndex &gi);
```

#### Step 2: Implement in `physics.cpp`

```cpp
GridIndex find_grid_index(const NuclearData &data,
                          const NuclideDescriptor &nuc, double E) {
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
  if (j >= rxn.xs_length - 1)
    return data.xs_pool[rxn.xs_offset + rxn.xs_length - 1];
  double xs_lo = data.xs_pool[rxn.xs_offset + j];
  double xs_hi = data.xs_pool[rxn.xs_offset + j + 1];
  return xs_lo + gi.t * (xs_hi - xs_lo);
}
```

#### Step 3: Refactor `total_macro_xs` in `material.cpp`

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

#### Step 4: Apply the same pattern in `sample_reaction`

```cpp
ReactionSample sample_reaction(const Material &mat, const NuclearData &data,
                               double E, double sigma_t, RNG &rng) {
  double xi = uniform(rng) * sigma_t;
  double cumulative = 0.0;

  for (int i = 0; i < mat.n_nuclides; ++i) {
    int nid = mat.nuclide_ids[i];
    const auto &nuc = data.nuclides[nid];
    double N = mat.number_densities[i];
    GridIndex gi = find_grid_index(data, nuc, E);  // ONE search per nuclide

    for (int r = 0; r < nuc.n_reactions; ++r) {
      const auto &rxn = data.reactions[nuc.rxn_offset + r];
      cumulative += N * lookup_xs_fast(data, rxn, gi);
      if (cumulative >= xi)
        return {nid, nuc.rxn_offset + r, static_cast<RxnType>(rxn.type)};
    }
  }
  // ... fallback unchanged ...
}
```

---

## 3. No Multithreading

**Impact: Linear with core count (4-16x on typical hardware)**

### Problem

The entire transport simulation is single-threaded. The event-based loop
structure (`event_advance` / `event_sample_reaction` / `event_process_collision`)
iterates over the particle bank serially. Monte Carlo particle transport is
embarrassingly parallel -- each particle's advance and collision is independent
(modulo the shared RNG).

### Fix

#### Step 1: Give each thread its own RNG

The current code uses a single `std::mt19937_64`. For threading, each thread
needs an independent RNG:

```cpp
// transport.h
struct TransportState {
  // ... existing fields ...
  std::vector<RNG> thread_rngs;  // one per thread
};
```

Initialize in `run_eigenvalue`:

```cpp
int n_threads = omp_get_max_threads();
state.thread_rngs.resize(n_threads);
for (int t = 0; t < n_threads; ++t)
  state.thread_rngs[t] = RNG(seed + t * 1000003ULL);
```

#### Step 2: Parallel event loops with OpenMP

```cpp
void event_advance(TransportState &state) {
  #pragma omp parallel for schedule(static)
  for (int i = 0; i < (int)state.current_bank.size(); ++i) {
    auto &n = state.current_bank[i];
    RNG &rng = state.thread_rngs[omp_get_thread_num()];
    n.sigma_t = total_macro_xs(*state.material, *state.data, n.E);
    double d = -std::log(uniform(rng)) / n.sigma_t;
    n.x += d * n.Omega_x;
    n.y += d * n.Omega_y;
    n.z += d * n.Omega_z;
  }
}
```

Apply the same `#pragma omp parallel for` to `event_sample_reaction` and
`event_process_collision`.

#### Step 3: Handle fission bank writes with thread-local banks

`fission()` and `multiply()` push to shared banks. Use thread-local banks
merged after the event to avoid all locking:

```cpp
// In event_process_collision:
std::vector<ParticleBank> thread_fission_banks(omp_get_max_threads());
std::vector<ParticleBank> thread_secondary_banks(omp_get_max_threads());

#pragma omp parallel for schedule(dynamic, 64)
for (int i = 0; i < n; ++i) {
  int tid = omp_get_thread_num();
  // ... process collision ...
  // fission_bank.push_back(sec) -> thread_fission_banks[tid].push_back(sec)
  // current_bank.push_back(sec) -> thread_secondary_banks[tid].push_back(sec)
}

// Serial merge
for (auto &tb : thread_fission_banks) {
  state.fission_bank.insert(state.fission_bank.end(), tb.begin(), tb.end());
  tb.clear();
}
for (auto &tb : thread_secondary_banks) {
  state.current_bank.insert(state.current_bank.end(), tb.begin(), tb.end());
  tb.clear();
}
```

#### Step 4: Add `-fopenmp` to Makefile

```makefile
CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -march=native -ffast-math -fopenmp -I include
```

#### Step 5: Parallel reductions for k_eff weight sums

```cpp
double W_source = 0.0;
#pragma omp parallel for reduction(+:W_source)
for (int i = 0; i < (int)state.current_bank.size(); ++i)
  W_source += state.current_bank[i].w;
```

---

## 4. Slow and Cache-Hostile RNG

**Impact: ~5-15% of total runtime**

### Problem

`std::mt19937_64` has a **2.5 KB internal state** (312 x 8-byte words). This
is terrible for cache performance when the RNG is called millions of times in
tight loops. Every call to `uniform()` at `rng.h:9` also constructs a new
`std::uniform_real_distribution` object -- while the compiler may optimise this
away, it's not guaranteed.

Each neutron history consumes ~10-20 random numbers per collision, with
thousands of particles and dozens of collisions per history. That's millions of
RNG calls per cycle.

### Fix

Replace with a lightweight, high-quality RNG. Recommended: **SplitMix64**
(8 bytes of state, passes BigCrush, fast enough for MC transport).

```cpp
// rng.h
#ifndef RNG_H
#define RNG_H

#include <cstdint>

struct RNG {
  uint64_t s;

  explicit RNG(uint64_t seed = 0) : s(seed) {}

  uint64_t next() {
    s += 0x9e3779b97f4a7c15ULL;
    uint64_t z = s;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
  }
};

inline double uniform(RNG &rng) {
  // Map upper 53 bits to [0, 1)
  return (rng.next() >> 11) * 0x1.0p-53;
}

#endif
```

Benefits:
- **~3-5x faster** than mt19937_64 per sample
- **8 bytes** of state vs 2.5 KB -- fits in a register
- Trivially embeddable per-particle for GPU porting
- Passes BigCrush -- sufficient quality for MC transport
- The existing `uniform(rng)` API is unchanged -- all call sites work as-is

---

## 5. Unbuffered Binary I/O in `score_flux`

**Impact: ~10-30% for runs with scoring enabled**

### Problem

`score_flux` (`transport.cpp:87-98`) performs two 8-byte `write()` calls per
alive neutron per collision:

```cpp
state.tally_file.write(reinterpret_cast<const char *>(&neutron.E), sizeof(double));
state.tally_file.write(reinterpret_cast<const char *>(&score), sizeof(double));
```

With 10,000 particles, ~50 collisions/history, 100 active cycles, that's ~100
million `write()` calls. Even with OS buffering, the per-call overhead of
`std::ofstream::write` (virtual dispatch, sentry construction, buffer management)
adds up.

### Fix

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

#### Step 3: Flush at end of each cycle

After the inner `while` loop in `run_eigenvalue`:

```cpp
if (!state.tally_buffer.empty()) {
  state.tally_file.write(
      reinterpret_cast<const char *>(state.tally_buffer.data()),
      static_cast<std::streamsize>(state.tally_buffer.size() * sizeof(double)));
  state.tally_buffer.clear();
}
```

#### Step 4: Pre-allocate the buffer

Before the cycle loop:

```cpp
state.tally_buffer.reserve(n_particles * 100);  // ~50 collisions * 2 doubles
```

---

## 6. Vector Reallocation in Fission/Multiply Events

**Impact: ~3-5% from allocator pressure**

### Problem

`fission()` (`reaction.cpp:73`) calls `fission_bank.push_back(secondary)` for
each fission neutron. `multiply()` (`reaction.cpp:107`) pushes to
`current_bank`. Neither bank is pre-reserved, causing repeated reallocations.

With 10,000 source particles and ~2.5 fission neutrons average, the fission
bank grows to ~25,000 per cycle. Starting empty, `std::vector` reallocates
~15 times (doubling from 1 to 32768).

### Fix

#### Step 1: Reserve fission bank before each cycle

```cpp
// transport.cpp -- run_eigenvalue, before the while loop
state.fission_bank.clear();
state.fission_bank.reserve(static_cast<size_t>(n_particles * 3));
```

#### Step 2: Reserve current bank headroom for (n,2n)/(n,3n) secondaries

```cpp
// Before event_process_collision
state.current_bank.reserve(state.current_bank.size() +
                           state.current_bank.size() / 10);  // ~10% headroom
```

---

## 7. Trig Function Optimisation

**Impact: ~3-5% of runtime in scatter routines**

### Problem

`sample_isodir` (`physics.cpp:89`) and `rotate_dir` (`physics.cpp:101`) call
`std::cos` and `std::sin` separately on the same angle. The CPU can compute
both simultaneously via `sincos`, but the compiler may or may not fuse them.

`rotate_dir` is called for every scattering event (elastic, inelastic,
Kalbach-Mann) -- the most common collision outcomes.

### Fix

#### Step 1: Use `sincos` explicitly

```cpp
void sample_isodir(double &Omega_x, double &Omega_y, double &Omega_z, RNG &rng) {
  double mu = 2 * uniform(rng) - 1;
  double varphi = 2 * micromc::pi * uniform(rng);
  double sin_theta = std::sqrt(1.0 - mu * mu);
  double cv, sv;
  sincos(varphi, &sv, &cv);  // POSIX, available on Linux/macOS
  Omega_x = sin_theta * cv;
  Omega_y = sin_theta * sv;
  Omega_z = mu;
}
```

#### Step 2: Apply the same in `rotate_dir`

Replace:
```cpp
double cos_varphi = std::cos(varphi);
double sin_varphi = std::sin(varphi);
```
With:
```cpp
double cos_varphi, sin_varphi;
sincos(varphi, &sin_varphi, &cos_varphi);
```

#### Step 3: Remove the renormalisation at the end of `rotate_dir`

Lines 135-139 compute `norm = sqrt(Ox^2+Oy^2+Oz^2)` and divide. This is a
safety net that costs 1 sqrt + 3 divides per scatter. The rotation formulas
preserve unit vectors (given the clamp on `mu`), so this can be removed or
guarded by `#ifndef NDEBUG` for debug-only assertion:

```cpp
#ifndef NDEBUG
  double norm = std::sqrt(Omega_x*Omega_x + Omega_y*Omega_y + Omega_z*Omega_z);
  assert(std::abs(norm - 1.0) < 1e-10);
#endif
```

---

## 8. `event_compact_bank` Efficiency

**Impact: ~2-5%**

### Problem

`event_compact_bank` (`transport.cpp:100-104`) uses `std::partition` + `erase`
on the entire bank every transport step. In early transport steps, very few
neutrons have died (most collisions are scattering). Partitioning 10,000
elements when only ~50 died is wasteful -- you're moving 72-byte structs
for every swap.

### Fix

**Option A: Skip dead particles instead of compacting.**

Add `if (!n.alive) continue;` at the top of `event_advance` and
`event_sample_reaction`. Only compact once after the bank is mostly dead
(e.g. when >25% are dead):

```cpp
void event_compact_bank(ParticleBank &bank) {
  int n_dead = std::count_if(bank.begin(), bank.end(),
                              [](const Neutron &n) { return !n.alive; });
  if (n_dead * 4 > static_cast<int>(bank.size())) {
    auto it = std::remove_if(bank.begin(), bank.end(),
                              [](const Neutron &n) { return !n.alive; });
    bank.erase(it, bank.end());
  }
}
```

**Option B: Use `std::remove_if` instead of `std::partition`.**

`remove_if` uses moves (cheaper than swaps for trivially-copyable types):

```cpp
void event_compact_bank(ParticleBank &bank) {
  bank.erase(
    std::remove_if(bank.begin(), bank.end(),
                   [](const Neutron &n) { return !n.alive; }),
    bank.end());
}
```

---

## 9. `comb_bank` Allocates Every Cycle

**Impact: ~1-2%**

### Problem

`comb_bank` (`transport.cpp:61-85`) creates a new `ParticleBank combed`, fills
it, then `std::move`-assigns it to the bank. This allocates ~800KB per cycle
(10,000 particles * ~80 bytes). Over 120 cycles, that's ~96MB of heap churn.

### Fix

Reuse a persistent scratch buffer in `TransportState`:

```cpp
// transport.h
struct TransportState {
  // ... existing ...
  ParticleBank comb_scratch;  // reusable buffer
};
```

```cpp
void comb_bank(ParticleBank &bank, int n_target, RNG &rng,
               ParticleBank &scratch) {
  double total_w = 0.0;
  for (const auto &n : bank) total_w += n.w;

  double step = total_w / n_target;
  double u = uniform(rng) * step;

  scratch.clear();
  // scratch already has capacity from previous cycle -- no allocation
  // ... same combing logic, writing into scratch ...

  std::swap(bank, scratch);
}
```

After the first cycle, `scratch` already has capacity and no allocation occurs.

---

## 10. AoS Particle Layout (Cache Inefficiency)

**Impact: ~10-20% improvement, enables SIMD and CUDA**

### Problem

`Neutron` is a struct of 10+ fields (~80 bytes with padding). The event-based
architecture processes one field at a time across all particles:

- `event_advance`: touches `E`, `x/y/z`, `Omega_x/y/z`, `sigma_t`
- `event_compact_bank`: touches `alive`
- `score_flux`: touches `E`, `w`, `alive`

With AoS layout, iterating over `E` for all particles pulls in all other fields
too, wasting cache lines. Each Neutron spans ~2 cache lines but a given event
often only needs 1-2 fields.

### Fix

This is a larger refactor noted as a future goal in CLAUDE.md.

#### Step 1: Define a SoA particle bank

```cpp
struct ParticleBank {
  std::vector<double> x, y, z;
  std::vector<double> E;
  std::vector<double> Omega_x, Omega_y, Omega_z;
  std::vector<double> w;
  std::vector<uint8_t> alive;       // avoid std::vector<bool> for perf
  std::vector<double> sigma_t;
  std::vector<ReactionSample> rxn;
  int size{0};
  int capacity{0};

  void reserve(int n);
  void push_back(/* all fields */);
  void clear() { size = 0; }
};
```

#### Step 2: Rewrite event loops

```cpp
void event_advance(TransportState &state) {
  auto &b = state.current_bank;
  for (int i = 0; i < b.size; ++i) {
    double d = -std::log(uniform(state.rng)) / b.sigma_t[i];
    b.x[i] += d * b.Omega_x[i];
    b.y[i] += d * b.Omega_y[i];
    b.z[i] += d * b.Omega_z[i];
  }
}
```

Now `x`, `y`, `z`, `Omega_x`, `Omega_y`, `Omega_z` are contiguous arrays --
the CPU prefetcher handles them efficiently and auto-vectorisation becomes
possible.

#### Step 3: Stream compaction instead of partition

```cpp
void event_compact_bank(ParticleBank &bank) {
  int write = 0;
  for (int read = 0; read < bank.size; ++read) {
    if (bank.alive[read]) {
      if (write != read) {
        bank.x[write] = bank.x[read];
        bank.y[write] = bank.y[read];
        // ... all fields ...
      }
      ++write;
    }
  }
  bank.size = write;
}
```

This is a prerequisite for both SIMD vectorisation and CUDA porting.

---

## 11. Iterator Invalidation in `multiply()` (Correctness Bug)

**Impact: Correctness risk -- undefined behaviour**

### Problem

`event_process_collision` (`transport.cpp:28`) takes a reference to a particle:

```cpp
Neutron &neutron = state.current_bank[i];
```

If `multiply()` at line 55 calls `current_bank.push_back(secondary)`, and the
push triggers a vector reallocation, `neutron` becomes a **dangling reference**.
Any subsequent read/write to `neutron` (which happens on the next loop
iteration via the `[i]` indexing) is undefined behaviour.

The loop captures `n = state.current_bank.size()` at the start, so new
particles aren't processed -- but the reallocation invalidates all existing
references and pointers into the vector.

### Fix

**Option A: Reserve before the loop** (quick, pairs with fix #6):

```cpp
// Before event_process_collision's for-loop
state.current_bank.reserve(state.current_bank.size() +
                           state.current_bank.size() / 5);
```

This only works if you can bound the number of secondaries. For correctness
guarantee, option B is better.

**Option B: Use a separate secondary bank, merge after the loop.**

```cpp
void event_process_collision(TransportState &state) {
  int n = static_cast<int>(state.current_bank.size());
  ParticleBank secondaries;
  for (int i = 0; i < n; ++i) {
    Neutron &neutron = state.current_bank[i];
    // ... switch ...
    case RxnType::N2N:
    case RxnType::N3N:
      multiply(neutron, rxn, *state.data, secondaries, state.rng);
      break;
  }
  // Safe merge after loop -- no references into current_bank are live
  state.current_bank.insert(state.current_bank.end(),
                            secondaries.begin(), secondaries.end());
}
```

---

## Summary: Implementation Priority

| # | Bottleneck | Estimated Speedup | Effort | Dependencies |
|---|---|---|---|---|
| 11 | Fix `multiply()` invalidation (correctness) | N/A | Low | None |
| 1 | Cache sigma_t (3x redundant lookup) | 2-3x | Low | None |
| 2 | One binary search per nuclide | 1.5-2x | Low | None |
| 3 | OpenMP threading | 4-16x (core count) | Medium | #4 (per-thread RNG) |
| 4 | Replace mt19937_64 with SplitMix64 | 1.05-1.15x | Low | None |
| 5 | Buffer tally I/O | 1.10-1.30x (scoring) | Low | None |
| 6 | Reserve fission/multiply banks | 1.03-1.05x | Trivial | None |
| 7 | sincos + remove renorm | 1.03-1.05x | Low | None |
| 8 | Smarter bank compaction | 1.02-1.05x | Low | None |
| 9 | Reuse comb scratch buffer | 1.01-1.02x | Trivial | None |
| 10 | AoS to SoA conversion | 1.10-1.20x | High | None (enables CUDA) |

### Recommended Implementation Order

1. **Fix #11 first** -- correctness bug, undefined behaviour.
2. **Apply #1 + #2 + #4 + #6 + #7 together** -- all low-effort, big cumulative
   gain. Estimated combined single-threaded speedup: **~4-6x**.
3. **Add #5** if scoring is used.
4. **Add OpenMP (#3)** -- needs per-thread RNG from #4. Multiplies by core count.
5. **SoA conversion (#10)** when preparing for CUDA.

**Bottom line:** Fixes #1 + #2 alone should yield ~3-5x single-threaded.
Adding OpenMP on 8 cores brings the total to **~20-40x**.
