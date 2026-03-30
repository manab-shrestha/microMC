from mendeleev import isotope, element
import os
import re

data_dir = "/home/ms3281/endfb-vii.1-hdf5/neutron/"
files = sorted(os.listdir(data_dir))

# Match e.g. H1.h5, U235.h5, Ag110_m1.h5 — skip c_* (thermal scattering)
pattern = re.compile(r'^([A-Z][a-z]?)(\d+)(_m(\d+))?\.h5$')

entries = []

for f in files:
    m = pattern.match(f)
    if not m:
        continue
    sym = m.group(1)
    A = int(m.group(2))
    meta_level = int(m.group(4)) if m.group(4) else 0

    el = element(sym)
    Z = el.atomic_number

    # ZAID = Z*1000 + A + 300*m (MCNP metastable convention)
    zaid = Z * 1000 + A + 300 * meta_level

    if A == 0:
        # Natural element (e.g. C0) — use standard atomic weight
        aw = el.atomic_weight
    else:
        iso = isotope(sym, A)
        aw = iso.mass

    entries.append((zaid, aw))

# Sort by ZAID
entries.sort()

# Print C++ code
print("// Auto-generated from ENDF/B-VII.1 HDF5 library via mendeleev")
print("// ZAID = Z*1000 + A (+300 for metastable)")
print()
print("#include <unordered_map>")
print()
print("const std::unordered_map<int, double> ATOMIC_MASS = {")
for zaid, aw in entries:
    print(f"    {{{zaid:>6d}, {aw:.6f}}},")
print("};")
