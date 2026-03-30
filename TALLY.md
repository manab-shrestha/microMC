# Flux Tally Implementation Guide

## Physics Basis

The collision estimator for scalar flux comes from the relation between collision rate density and flux:

```
C(E) = Sigma_t(E) * phi(E)
```

Rearranging: `phi(E) = C(E) / Sigma_t(E)`. Each collision with weight `w` at energy `E` contributes `w / Sigma_t(E)` to the flux. This is scored **before** the reaction physics is applied — the incident energy is what matters, not the post-scatter energy.

---

## Data Structures

### In `include/transport.h`

Add `#include <fstream>` to the includes.

Add two fields to `TransportState`:

```cpp
std::ofstream tally_file;
bool scoring_active{false};
```

- `tally_file` — binary output stream for writing (E, score) pairs
- `scoring_active` — flag set to `true` only during active cycles

### Declaration

Add to `include/transport.h`:

```cpp
void score_flux(TransportState &state);
```

---

## Implementation

### `score_flux` in `source/transport.cpp`

```cpp
void score_flux(TransportState &state) {
  for (const auto &neutron : state.current_bank) {
    if (!neutron.alive) continue;

    double sigma_t = total_macro_xs(*state.material, *state.data, neutron.E);
    double score = neutron.w / sigma_t;

    state.tally_file.write(reinterpret_cast<const char*>(&neutron.E), sizeof(double));
    state.tally_file.write(reinterpret_cast<const char*>(&score), sizeof(double));
  }
}
```

Each call writes one (E, score) pair per alive neutron in the bank. The binary format avoids the overhead of text formatting and keeps the output compact.

---

## Integration into `run_eigenvalue`

### 1. Open the file before the cycle loop

```cpp
state.tally_file.open("flux_tally.bin", std::ios::binary);
```

Place this after the `TransportState` initialisation and source bank population, before the cycle for-loop.

### 2. Set the scoring flag each cycle

At the top of the cycle loop body:

```cpp
state.scoring_active = (c >= n_inactive);
```

### 3. Call `score_flux` in the transport while loop

Insert `score_flux` **before** `event_process_collision` and only when scoring is active:

```
while (!state.current_bank.empty()) {
    event_advance(state);
    event_sample_reaction(state);
    if (state.scoring_active) score_flux(state);
    event_process_collision(state);
    event_compact_bank(state.current_bank);
}
```

The ordering matters: `score_flux` must come after `event_advance` (so the neutron has collided) and after `event_sample_reaction` (so the reaction is sampled), but before `event_process_collision` (so we score at the incident energy, not the post-scatter energy).

### 4. Close the file after the cycle loop

```cpp
state.tally_file.close();
```

---

## Binary Output Format

Sequential pairs of `double` values, 16 bytes per entry:

```
[E_0 : 8 bytes][score_0 : 8 bytes][E_1 : 8 bytes][score_1 : 8 bytes] ...
```

Total file size = 16 * (number of collisions during active cycles) bytes.

---

## Python Post-Processing

### Read the binary data

```python
import numpy as np
import matplotlib.pyplot as plt

data = np.fromfile("flux_tally.bin", dtype=np.float64).reshape(-1, 2)
E = data[:, 0]       # energy in eV
score = data[:, 1]    # w / Sigma_t
```

### Bin into energy groups

```python
bins = np.logspace(-2, 7, 200)  # 0.01 eV to 10 MeV, 200 bins
flux, edges = np.histogram(E, bins=bins, weights=score)
```

### Normalise per unit lethargy

```python
lethargy_width = np.log(edges[1:] / edges[:-1])
flux_per_lethargy = flux / lethargy_width
```

### Normalise per source neutron

```python
n_particles = 1000    # match your run_eigenvalue argument
n_active = 100        # match your run_eigenvalue argument
flux_per_lethargy /= (n_particles * n_active)
```

### Plot

```python
bin_centres = np.sqrt(edges[:-1] * edges[1:])  # geometric mean
plt.semilogx(bin_centres, flux_per_lethargy)
plt.xlabel("Energy (eV)")
plt.ylabel("Flux per unit lethargy (arbitrary)")
plt.title("Neutron flux spectrum")
plt.grid(True, alpha=0.3)
plt.savefig("flux_spectrum.png", dpi=150)
plt.show()
```

### Compare with Serpent

To compare with Serpent's energy-binned flux detector, use the same group structure in both codes and normalise both spectra to integrate to 1 (or compare relative shapes).

---

## Expected Output

For the UO2+H2O mixture at these number densities, expect:
- A thermal peak around 0.025-0.1 eV (Maxwell spectrum at 900K)
- A 1/E slowing-down region from ~1 eV to ~100 keV
- Resonance dips at U238 resonance energies (6.67 eV, 20.9 eV, 36.7 eV, etc.)
- A fission spectrum tail above ~100 keV peaking around 1-2 MeV

---

## File Size Estimate

With 1000 particles, ~50 collisions per history, 100 active cycles:
- ~5,000,000 collision events
- ~80 MB binary file

With 10000 particles: ~800 MB. Consider reducing active cycles or increasing bin width if file size is a concern.
