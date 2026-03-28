# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

```bash
make          # build ./micromc binary (outputs to project root)
make clean    # remove build/ and binary
./micromc     # run (requires xs/bin/nuclear_data.bin to exist)
```

The Makefile compiles all `source/*.cpp` with C++17 (`-std=c++17 -Wall -Wextra -O2 -I include`), linking into `./micromc`. Object and dependency files go in `build/`.

## Nuclear data pipeline

Before running the C++ code, the HDF5 nuclear data must be extracted to a flat binary:

```bash
cd xs
source ../venv/bin/activate   # h5py, numpy
python extract_hdf5.py        # reads hdf5/*.h5, writes bin/nuclear_data.bin
```

HDF5 source files live in `xs/hdf5/` (OpenMC format, ENDF/B-VII.1, `900K` temperature group). The extractor converts them to `xs/bin/nuclear_data.bin`, which the C++ loader expects at that exact relative path.

## Architecture

The codebase is structured around a flat, SoA-friendly nuclear data representation designed for eventual CUDA porting.

**Data flow:**
1. `xs/extract_hdf5.py` — reads OpenMC HDF5 files and writes a single flat binary blob
2. `source/NuclearData.cpp` + `include/NuclearData.h` — binary loader; produces `NuclearDataHost` (std::vector owner) and `NuclearData` (raw-pointer view into it, device-compatible)
3. `source/main.cpp` — loads data, queries macroscopic XS, currently exercises the data structures

**Key design decisions:**
- `NuclearData` is a pure-POD view struct (no std:: types) so it can be copied to GPU device memory unchanged. `NuclearDataHost` owns the heap via `std::vector`; `host.view()` returns a `NuclearData` pointing into those vectors.
- All distribution data (angular, fission energy, Kalbach-Mann, fission yields) is stored in typed pool structs (`AngularDistPool`, `EnergyDistPool`, `KalbachMannDistPool`, `FissionYieldPool`), each a flat array with offset/length indirection — no nested allocation.
- `ReactionDescriptor.dist_id` indexes into the correct pool for that reaction type. `yield_id` is only valid for fission (`RxnType::FISSION`); it is -1 otherwise.
- XS lookup uses a per-nuclide energy grid (linear interpolation). `threshold_idx` marks the first grid point where a reaction's XS is non-zero; `xs_pool` stores only from that point onward.

**Reaction channels (RxnType enum):**
- `ELASTIC` (MT=2): angular dist in `AngularDistPool`
- `DISCRETE_INELASTIC` (MT=51–90): angular dist in `AngularDistPool`, Q-value in descriptor
- `CONTINUUM_INELASTIC` (MT=91): Kalbach-Mann dist in `KalbachMannDistPool`
- `FISSION` (MT=18): energy dist in `EnergyDistPool`, nu-bar in `FissionYieldPool`, `multiplicity=-1` signals energy-dependent yield
- `ABSORPTION` (MT=102–117): summed into one channel, no secondaries, `dist_id=-1`
- `N2N`/`N3N` (MT=16/17): Kalbach-Mann dist, constant multiplicity 2/3

**Current nuclides:** H1 (1001), O16 (8016), U235 (92235), U238 (92238) — UO2+H2O mix.

**Particle struct** (`include/neutron.h`): position (x,y,z), energy E, direction (Omega_x/y/z), weight w, alive flag — currently AoS, SoA conversion is a future goal.
