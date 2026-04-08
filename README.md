# microMC

A Monte Carlo neutron transport code for k-eigenvalue and fixed-source calculations. Designed with a flat, SoA-friendly data layout for eventual CUDA porting.

## Dependencies

### C++ (system)

- **C++17** compiler (g++ or clang++)
- **HDF5** with C++ bindings

```bash
# macOS
brew install hdf5

# Ubuntu/Debian
sudo apt install libhdf5-dev
```

- **bear** (optional, for generating `compile_commands.json` for LSP support)

```bash
brew install bear      # macOS
sudo apt install bear  # Ubuntu
```

### Python

Required for the tally plotting scripts in `tallies/`.

```bash
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

## Nuclear data

Nuclear data files are not included in the repository. The code expects ENDF/B-VII.1 HDF5 files in OpenMC format. Set the path in `source/main.cpp`:

```cpp
const std::string xs_path = "/path/to/endfb-vii.1-hdf5/neutron";
```

## Build

```bash
make          # build ./micromc binary
make clean    # remove build artifacts
make compdb   # regenerate compile_commands.json (requires bear)
```

## Run

```bash
./micromc          # k-eigenvalue calculation
./micromc true     # with flux tally output
```

Tally output is written to `tallies/`. Use the plotting scripts to visualise:

```bash
source venv/bin/activate
python tallies/plot_flux.py
```

## Architecture

See `CLAUDE.md` for a detailed description of the code architecture, data structures, and design decisions.
