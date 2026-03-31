# Code Cleanup Guide

A non-performance review of naming, structure, clarity, and C++ practices.
Organised by category, each item references the specific file and line.

---

## 1. Naming Inconsistencies

### 1.1 No consistent casing convention

The codebase mixes several naming styles with no clear rule:

| Style | Examples |
|---|---|
| PascalCase | `NuclearData`, `Material`, `TransportState`, `GridIndex` |
| snake_case | `total_macro_xs`, `binary_search`, `sample_reaction` |
| Mixed | `event_calc_XS`, `Sigma_t`, `lookup_xs_improved` |
| UPPER | `MAX_NUCLIDES_PER_MATERIAL`, `ATOMIC_MASS` |

**Recommendation:** Pick one convention and apply it everywhere:
- **Types/structs**: PascalCase (already consistent)
- **Functions**: snake_case (already mostly consistent)
- **Variables/fields**: snake_case (fix `Sigma_t` -> `sigma_t`, `W_source` -> `w_source`, etc.)
- **Constants**: UPPER_SNAKE or `k_` prefix constexpr

Specific renames needed:

| Current | Proposed | Location |
|---|---|---|
| `event_calc_XS` | `event_calc_xs` | `transport.cpp:10` |
| `Sigma_t` (field) | `sigma_t` | `neutron.h:19` |
| `W_source` | `w_source` | `transport.cpp:143` |
| `W_fission` | `w_fission` | `transport.cpp:159` |
| `lookup_xs_improved` | `lookup_xs_at` | `physics.cpp:37` (see 1.3) |

### 1.2 Overloaded `n`

The letter `n` is used for at least four different things:

| Meaning | Location |
|---|---|
| A Neutron reference | `transport.cpp:11` (`for (auto &n : state.current_bank)`) |
| Array length | `physics.cpp:29` (`int n = nuc.grid_length`) |
| Bank size | `transport.cpp:36` (`int n = static_cast<int>(...)`) |
| A Neutron in comb_bank | `transport.cpp:75` (`for (const auto &n : bank)`) |

When `n` means "neutron" in one function and "count" in the next, it forces the
reader to re-derive context every time.

**Recommendation:**
- Neutron loop variables: `p` (particle) or `neutron`
- Counts/sizes: `n_points`, `n_bins`, `bank_size`, etc.
- Keep single-letter names only for genuinely local, short-lived math variables
  (like `i`, `j`, `t` in interpolation)

### 1.3 `lookup_xs_improved` is not descriptive

`physics.cpp:37` — The name says *how it was created* (it's the improved
version), not *what it does* (look up XS using a pre-computed grid index). If
you come back in 6 months, "improved" tells you nothing.

**Rename to:** `lookup_xs_at` or `lookup_xs_at_grid_index`

This also clarifies the relationship: `find_grid_index` finds the index,
`lookup_xs_at` uses it.

### 1.4 `GridIndex` fields are too terse

```cpp
struct GridIndex {
  int i;     // grid bin index
  double t;  // interpolation fraction
};
```

`i` and `t` are fine in a 3-line interpolation, but this struct gets passed
around between functions and files. Slightly more descriptive names help:

```cpp
struct GridIndex {
  int bin;
  double frac;  // interpolation fraction within bin
};
```

### 1.5 Material name is `"blob"`

`main.cpp:31` — The material is described in a comment as "identical to NE8
CW1 and CW2" UO2+H2O, but named `"blob"`. This will confuse anyone reading
output or error messages.

**Rename to:** `"UO2_H2O"` or `"fuel_moderator"`.

### 1.6 `nid` abbreviation

`physics.cpp:77` — `nid` for "nuclide id" saves 7 characters but hurts
readability. Use `nuclide_idx` to match the `nuclide_ids` array it indexes
into.

---

## 2. File & Header Structure

### 2.1 Inconsistent header file naming

| File | Style |
|---|---|
| `NuclearData.h` | PascalCase |
| `Material.h` | PascalCase |
| `neutron.h` | lowercase |
| `physics.h` | lowercase |
| `rng.h` | lowercase |
| `transport.h` | lowercase |
| `reaction.h` | lowercase |
| `atomic_mass.h` | snake_case |

**Recommendation:** Pick one. Since most headers are lowercase, rename
`NuclearData.h` -> `nuclear_data.h` and `Material.h` -> `material.h`. This
also matches the corresponding `.cpp` filenames (`nuclear_data.cpp`,
`material.cpp`).

### 2.2 Inconsistent include paths

Three different include styles are used for project headers:

```cpp
// neutron.h — angle brackets for project header (wrong)
#include <NuclearData.h>

// transport.cpp — relative path with ../
#include "../include/transport.h"

// material.cpp — bare name (relies on -I include)
#include "Material.h"
```

**Recommendation:** Use quotes with bare names everywhere, relying on
`-I include` which is already in the Makefile:

```cpp
#include "nuclear_data.h"
#include "material.h"
#include "transport.h"
```

Angle brackets (`<>`) should be reserved for system/library headers.

### 2.3 `physics.cpp` is a grab-bag

At 267 lines, `physics.cpp` contains:

- Binary search utility
- Energy grid lookup (`find_grid_index`, `lookup_xs`, `lookup_xs_improved`)
- Direction sampling (`sample_isodir`, `rotate_dir`)
- Reaction channel sampling (`sample_reaction`)
- Tabulated CDF sampling (`sample_tab_cdf`)
- Angular distribution sampling (`sample_cosine`)
- Kalbach-Mann sampling (`sample_kalbach_mann`)
- Fission energy sampling (`sample_fission_energy`)
- Nu-bar sampling (`sample_nu_bar`)
- Stochastic energy interpolation (`stoch_energy_interp`)
- A private namespace with `pi` and `sign`

These are logically separate concerns. **Recommended split:**

| New file | Contents |
|---|---|
| `xs_lookup.cpp/.h` | `binary_search`, `find_grid_index`, `lookup_xs`, `lookup_xs_at`, `total_macro_xs` |
| `sampling.cpp/.h` | `sample_tab_cdf`, `sample_cosine`, `sample_kalbach_mann`, `sample_fission_energy`, `sample_nu_bar`, `stoch_energy_interp`, `sample_reaction` |
| `geometry.cpp/.h` | `sample_isodir`, `rotate_dir` (direction math) |

This way, when you're debugging a XS lookup issue you open one file, not a file
that also has Kalbach-Mann math in it.

### 2.4 `atomic_mass.h` bloats every translation unit

`atomic_mass.h` defines a ~150-entry `inline const std::unordered_map` in a
header. Every translation unit that includes `Material.h` (which includes
`atomic_mass.h`) gets this map inlined. The `inline` keyword prevents multiple
definition errors, but the compiler still has to process the entire initialiser
list in every TU.

**Recommendation:** Move the map to `atomic_mass.cpp` and expose only a lookup
function:

```cpp
// atomic_mass.h
#pragma once
namespace mendeleev {
double atomic_mass(int zaid);  // throws if not found
}

// atomic_mass.cpp
#include "atomic_mass.h"
#include <unordered_map>
#include <stdexcept>

namespace mendeleev {
static const std::unordered_map<int, double> TABLE = { ... };

double atomic_mass(int zaid) {
  auto it = TABLE.find(zaid);
  if (it == TABLE.end()) throw std::runtime_error("Unknown ZAID: " + std::to_string(zaid));
  return it->second;
}
}
```

This encapsulates the data, speeds up compilation, and removes the
`<unordered_map>` include from every header chain.

---

## 3. C++ Practice Issues

### 3.1 Angle brackets for project headers

`neutron.h:5`:
```cpp
#include <NuclearData.h>
```

Angle brackets tell the compiler to search system include paths first. This
happens to work because of `-I include`, but it's semantically wrong and
confusing. Use quotes: `#include "NuclearData.h"`.

### 3.2 Trailing semicolon after namespace

`physics.cpp:12`:
```cpp
}; // namespace micromc
```

The semicolon after a closing namespace brace is unnecessary and technically a
null declaration. Most style guides flag this. Remove it:
```cpp
} // namespace micromc
```

### 3.3 `ReactionDescriptor.type` should be `RxnType`, not `int`

`NuclearData.h:30`:
```cpp
struct ReactionDescriptor {
  int type;      // RxnType as int
```

This forces `static_cast<RxnType>(rxn.type)` at every use site
(`transport.cpp:46`, `physics.cpp:86`). Since `RxnType` is backed by `int`
already (`enum class RxnType : int`), just store it directly:

```cpp
struct ReactionDescriptor {
  RxnType type;
```

The binary loader in `nuclear_data.cpp` reads an `int` and can cast once:
```cpp
int raw_type;
read_i32(f, raw_type);
rd.type = static_cast<RxnType>(raw_type);
```

This eliminates every `static_cast<RxnType>(rxn.type)` scatter.

### 3.4 `const char *name` in Material

`Material.h:11`:
```cpp
const char *name;
```

Raw C strings are error-prone (no ownership, can dangle). Since `Material` is
only constructed with string literals, `std::string_view` is the safe
zero-cost replacement:

```cpp
std::string_view name;
```

### 3.5 Implicit double-to-int conversion

`main.cpp:47`:
```cpp
run_eigenvalue(fuel, data, 1e4, 20, 100, 123, flux_detector);
```

`1e4` is a `double`, but `n_particles` is `int`. This is an implicit narrowing
conversion. Some compilers warn, some don't. Use `10000` or `10'000`.

### 3.6 Integer division truncates elapsed time

`main.cpp:53-54`:
```cpp
std::cout << "k-eigenvalue calculation completed in: "
          << elapsed.count() / 1000 << " seconds\n";
```

If the run takes 1999ms, this prints "1 seconds". Use
`std::chrono::duration_cast<std::chrono::seconds>` or print with a decimal:

```cpp
std::cout << "completed in: "
          << elapsed.count() / 1000.0 << " seconds\n";
```

### 3.7 Unused includes in main.cpp

`main.cpp:8-10`:
```cpp
#include <ratio>
#include <stdexcept>
#include <type_traits>
```

`<ratio>` and `<type_traits>` are not used. `<stdexcept>` is only needed
indirectly (the exceptions are thrown in other TUs, caught via
`std::exception`). Remove unused includes.

### 3.8 `GridIndex` parameter should be const reference

`physics.h:24`:
```cpp
double lookup_xs_improved(const NuclearData &data,
                          const ReactionDescriptor &rxn, GridIndex &gi);
```

`gi` is not modified — this should be `const GridIndex &gi`. A non-const
reference parameter signals to the reader that the function mutates it.

### 3.9 `bool` comparison is redundant

`main.cpp:16`:
```cpp
flux_detector = (std::string(argv[1]) == "true") ? true : false;
```

The `== "true"` comparison already returns `bool`. Simplify:
```cpp
flux_detector = (std::string(argv[1]) == "true");
```

### 3.10 Use `#pragma once` instead of include guards

Every header uses manual include guards:
```cpp
#ifndef NUCLEAR_DATA_H
#define NUCLEAR_DATA_H
...
#endif
```

`#pragma once` is supported by every major compiler (GCC, Clang, MSVC) and
eliminates the risk of guard name collisions. It's also less boilerplate:

```cpp
#pragma once
```

This is a stylistic preference — the current guards work fine. But if you're
cleaning up, it's a low-risk simplification.

---

## 4. Leftover Debris

### 4.1 Debug comment at end of main.cpp

`main.cpp:59`:
```cpp
// no opt: 257s
```

Leftover benchmarking note. Remove it.

### 4.2 Commented-out material fractions with stray backticks

`main.cpp:34`:
```cpp
// {0.4664, 0.3268, 0.00536, 0.1455},``
```

Old values with stray backticks. Remove the line entirely or if the values are
meaningful, explain why they were replaced.

### 4.3 Typo in comment

`main.cpp:29`:
```cpp
// Material identiacal to that in NE8 CW1 and CW2
```

"identiacal" -> "identical"

### 4.4 Stale comment in `physics.cpp`

`physics.cpp:99`:
```cpp
// This is how we will sample our various CDFs
```

"This is how we will" is a note-to-self from development. Either remove it or
rewrite as a doc comment:
```cpp
// Sample a value from a tabulated CDF using linear interpolation within bins.
```

---

## 5. Magic Numbers

Several numeric literals appear without explanation:

| Value | Location | Meaning |
|---|---|---|
| `5e6` | `transport.cpp:131` | Max initial source energy (eV) |
| `1e-14` | `physics.cpp:138` | Threshold for near-polar direction |
| `1e-8` | `physics.cpp:224` | Threshold for isotropic Kalbach limit |
| `-10.49` | `main.cpp:32` | UO2 mass density (g/cm3) |
| `0.602214076` | `material.cpp:45` | Avogadro in barn-cm units |
| `123` | `main.cpp:47` | RNG seed |

**Recommendation:** Define named constants for the physics ones:

```cpp
namespace micromc {
inline constexpr double pi = 3.14159265358979323846;
inline constexpr double N_A_BARN_CM = 0.602214076;  // Avogadro's in barn-cm
inline constexpr double POLAR_EPSILON = 1e-14;
inline constexpr double KALBACH_ISO_THRESHOLD = 1e-8;
}
```

The material density and seed are configuration, not magic numbers — those are
fine as literals but should eventually come from input.

---

## 6. Structural Suggestions

### 6.1 `micromc` namespace is barely used

Only `pi` and `sign` live in `namespace micromc` (inside `physics.cpp`). Every
other function and struct is in the global namespace. For a project of this
size, polluting the global namespace is manageable, but it's good practice to
wrap your types:

```cpp
namespace micromc {
  struct Neutron { ... };
  struct Material { ... };
  // etc.
}
```

This prevents collisions if you ever link against another library. At minimum,
be consistent: if `pi` is in `micromc::`, the functions that use it should be
too.

### 6.2 `run_eigenvalue` does too much

`transport.cpp:117-196` — This 80-line function handles:
- State initialisation
- Source generation
- The cycle loop
- k_eff accumulation and statistics
- Printing
- File I/O setup and teardown

Consider splitting into helpers:

```cpp
void init_source(TransportState &state, int n_particles);
void run_cycle(TransportState &state);
void print_cycle_summary(const TransportState &state, int cycle,
                          double k_sum, int n_active_so_far);
```

The cycle loop itself then becomes a clean high-level driver.

### 6.3 `event_calc_XS` is declared nowhere

`transport.cpp:10` defines `event_calc_XS` but it's not declared in
`transport.h`. It's only called within `transport.cpp` so it works, but this
means it can't be tested independently and doesn't match the pattern of the
other `event_*` functions which are all declared in the header.

Either add it to `transport.h` or make it `static`/anonymous-namespace to
signal it's file-local.

---

## Summary: Priority Order

| # | Item | Effort | Impact on readability |
|---|---|---|---|
| 4 | Remove leftover debris (comments, typos, backticks) | 5 min | Immediate |
| 3.3 | `type` field as `RxnType` (removes scattered casts) | 15 min | High |
| 1.1 | Fix `event_calc_XS` / `Sigma_t` / `W_source` casing | 10 min | Medium |
| 2.2 | Standardise include paths | 10 min | Medium |
| 1.2 | Stop using `n` for everything | 15 min | High |
| 1.3 | Rename `lookup_xs_improved` | 5 min | Medium |
| 3.7 | Remove unused includes | 2 min | Low |
| 3.5 | Fix `1e4` narrowing conversion | 1 min | Low |
| 3.8 | `const GridIndex &gi` | 1 min | Low |
| 2.1 | Consistent header file names | 15 min | Medium |
| 2.3 | Split `physics.cpp` | 30 min | High |
| 2.4 | Move atomic mass map to .cpp | 15 min | Medium |
| 5 | Name magic numbers | 10 min | Medium |
| 6.2 | Split `run_eigenvalue` | 20 min | High |
