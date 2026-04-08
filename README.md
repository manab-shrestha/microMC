# microMC

A toy Monte Carlo neutron transport code for k-eigenvalue and fixed-source calculations. Designed for eventual CUDA porting.

## Dependencies

### C++

- **C++17** compiler (g++ or clang++)
- **HDF5** with C++ bindings

```bash
# macOS
brew install hdf5

# Ubuntu/Debian
sudo apt install libhdf5-dev
```

- **bear** (optional but recommended for (n)vim users. bear generates `compile_commands.json` for LSP support)

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

Nuclear data files are not included in the repository. The code expects ENDF/B-VII.1 HDF5 files in OpenMC format, which can be downloaded [here](https://anl.box.com/shared/static/9igk353zpy8fn9ttvtrqgzvw1vtejoz6.xz).

After downloading, extract the archive:

```bash
tar -xJf 9igk353zpy8fn9ttvtrqgzvw1vtejoz6.xz
```

Then set `xs_path` in `source/main.cpp` to the extracted `neutron` directory:

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


Tally output is written to `tallies/`. Currently, only a flux spectrum tally is available; additional tallies will be implemented in future.

Use the plotting scripts to visualise the results:

```bash
# example
source venv/bin/activate
python tallies/plot_flux.py
```

