"""
Extract nuclear data from OpenMC HDF5 files into flat binary pools
matching the microMC SoA design.

Output directory: xs/bin/
  - nuclear_data.bin   : single binary blob containing all pools + header

Binary layout:
  Header (fixed size) → struct array pools → double pools → int pools
"""

import struct
import numpy as np
import h5py as h5
from pathlib import Path

TEMP = "900K"
OUTPUT_DIR = Path("bin")
FILE_MAGIC = 0x4D434442  # "MCDB"
FILE_VERSION = 1

# Nuclides to extract, in order. Each entry: (filename, ZAID)
NUCLIDES = [
    ("H1",   1001),
    ("O16",  8016),
    ("U235", 92235),
    ("U238", 92238),
]

# ── Reaction type enum (must match C++ RxnType) ─────────────────────
ELASTIC              = 0
DISCRETE_INELASTIC   = 1
CONTINUUM_INELASTIC  = 2
FISSION              = 3
ABSORPTION           = 4
N2N                  = 5
N3N                  = 6

# ── Pool accumulators ────────────────────────────────────────────────

class AngularDistPoolBuilder:
    """Accumulates angular distribution data (elastic + discrete inelastic)."""
    def __init__(self):
        self.energy_offsets = [0]  # n_dists + 1
        self.energies = []
        self.table_offsets = [0]   # total_energy_points + 1
        self.mu = []
        self.pdf = []
        self.cdf = []
        self.n_dists = 0

    def add(self, angle_group):
        """Add an angular distribution from an HDF5 angle group.
        Returns the dist_id for this distribution."""
        dist_id = self.n_dists
        self.n_dists += 1

        E = np.array(angle_group["energy"])
        mu_data = np.array(angle_group["mu"])      # shape (3, N)
        offsets = np.array(angle_group["mu"].attrs["offsets"])

        self.energies.extend(E.tolist())
        self.energy_offsets.append(len(self.energies))

        n_e = len(E)
        for i in range(n_e):
            start = offsets[i]
            end = offsets[i + 1] if i + 1 < len(offsets) else mu_data.shape[1]
            self.mu.extend(mu_data[0, start:end].tolist())
            self.pdf.extend(mu_data[1, start:end].tolist())
            self.cdf.extend(mu_data[2, start:end].tolist())
            self.table_offsets.append(len(self.mu))

        return dist_id


class EnergyDistPoolBuilder:
    """Accumulates energy distribution data (fission)."""
    def __init__(self):
        self.energy_offsets = [0]
        self.energies = []
        self.table_offsets = [0]
        self.E_out = []
        self.pdf = []
        self.cdf = []
        self.n_dists = 0

    def add(self, energy_group):
        """Add from an HDF5 energy distribution group.
        Returns the dist_id."""
        dist_id = self.n_dists
        self.n_dists += 1

        E = np.array(energy_group["energy"])
        dist_data = np.array(energy_group["distribution"])   # (3, N)
        offsets = np.array(energy_group["distribution"].attrs["offsets"])

        self.energies.extend(E.tolist())
        self.energy_offsets.append(len(self.energies))

        n_e = len(E)
        for i in range(n_e):
            start = offsets[i]
            end = offsets[i + 1] if i + 1 < len(offsets) else dist_data.shape[1]
            self.E_out.extend(dist_data[0, start:end].tolist())
            self.pdf.extend(dist_data[1, start:end].tolist())
            self.cdf.extend(dist_data[2, start:end].tolist())
            self.table_offsets.append(len(self.E_out))

        return dist_id


class KalbachMannDistPoolBuilder:
    """Accumulates Kalbach-Mann distribution data (continuum inelastic, n2n, n3n)."""
    def __init__(self):
        self.energy_offsets = [0]
        self.energies = []
        self.table_offsets = [0]
        self.E_out = []
        self.pdf = []
        self.cdf = []
        self.r = []
        self.a = []
        self.n_dists = 0

    def add(self, dist_group):
        """Add from an HDF5 Kalbach-Mann distribution_0 group.
        Returns the dist_id."""
        dist_id = self.n_dists
        self.n_dists += 1

        E = np.array(dist_group["energy"])
        dist_data = np.array(dist_group["distribution"])   # (5, N)
        offsets = np.array(dist_group["distribution"].attrs["offsets"])

        self.energies.extend(E.tolist())
        self.energy_offsets.append(len(self.energies))

        n_e = len(E)
        for i in range(n_e):
            start = offsets[i]
            end = offsets[i + 1] if i + 1 < len(offsets) else dist_data.shape[1]
            self.E_out.extend(dist_data[0, start:end].tolist())
            self.pdf.extend(dist_data[1, start:end].tolist())
            self.cdf.extend(dist_data[2, start:end].tolist())
            self.r.extend(dist_data[3, start:end].tolist())
            self.a.extend(dist_data[4, start:end].tolist())
            self.table_offsets.append(len(self.E_out))

        return dist_id


class FissionYieldBuilder:
    """Accumulates fission yield (nu-bar) data."""
    def __init__(self):
        self.entries = []  # list of (energy_offset, n_points) into the global arrays
        self.energies = []
        self.nu_bar = []

    def add(self, yield_array):
        """Add from a (2, N) yield array. Returns the yield_id."""
        yield_id = len(self.entries)
        offset = len(self.energies)
        n = yield_array.shape[1]
        self.energies.extend(yield_array[0].tolist())
        self.nu_bar.extend(yield_array[1].tolist())
        self.entries.append((offset, n))
        return yield_id


# ── Main extraction ──────────────────────────────────────────────────

def extract():
    # Global pools
    energy_grids = []       # all nuclide energy grids concatenated
    xs_pool = []            # all reaction XS concatenated

    nuclide_descriptors = []   # list of dicts
    reaction_descriptors = []  # list of dicts

    angular_pool = AngularDistPoolBuilder()
    fission_energy_pool = EnergyDistPoolBuilder()
    kalbach_pool = KalbachMannDistPoolBuilder()
    fission_yield_pool = FissionYieldBuilder()

    for name, zaid in NUCLIDES:
        print(f"Processing {name} (ZAID={zaid})...")
        with h5.File(f"hdf5/{name}.h5", "r") as f:
            nuc = f[name]
            awr = nuc.attrs["atomic_weight_ratio"]
            E_grid = np.array(nuc["energy"][TEMP])

            grid_offset = len(energy_grids)
            grid_length = len(E_grid)
            energy_grids.extend(E_grid.tolist())

            rxn_offset = len(reaction_descriptors)

            rxn_keys = sorted(nuc["reactions"].keys(),
                              key=lambda x: int(x.split("_")[1]))

            # Collect non-fission absorption XS (MT=102..117)
            # We sum them into a single absorption channel
            absorption_xs = np.zeros(grid_length)
            has_absorption = False

            for rk in rxn_keys:
                r = nuc["reactions"][rk]
                mt = int(r.attrs["mt"])
                redundant = int(r.attrs.get("redundant", 0))

                if redundant:
                    continue

                # Skip MTs we don't care about for neutron transport
                if mt >= 200:
                    continue

                if 102 <= mt <= 117:
                    # Absorption channel — accumulate
                    xs_data = r[TEMP]["xs"]
                    xs = np.array(xs_data)
                    thresh = int(xs_data.attrs.get("threshold_idx", 0))
                    padded = np.zeros(grid_length)
                    padded[thresh:thresh + len(xs)] = xs
                    absorption_xs += padded
                    has_absorption = True
                    continue

                if mt == 2:
                    # Elastic scattering
                    xs_data = r[TEMP]["xs"]
                    xs = np.array(xs_data)
                    thresh = int(xs_data.attrs.get("threshold_idx", 0))

                    xs_off = len(xs_pool)
                    xs_pool.extend(xs.tolist())

                    # Angular distribution
                    angle = r["product_0"]["distribution_0"]["angle"]
                    dist_id = angular_pool.add(angle)

                    reaction_descriptors.append({
                        "type": ELASTIC,
                        "xs_offset": xs_off,
                        "xs_length": len(xs),
                        "threshold_idx": thresh,
                        "dist_id": dist_id,
                        "Q_value": 0.0,
                        "multiplicity": 1,
                    })

                elif mt == 18:
                    # Fission
                    xs_data = r[TEMP]["xs"]
                    xs = np.array(xs_data)
                    thresh = int(xs_data.attrs.get("threshold_idx", 0))

                    xs_off = len(xs_pool)
                    xs_pool.extend(xs.tolist())

                    # Prompt neutron energy distribution (product_0)
                    p0 = r["product_0"]
                    energy_group = p0["distribution_0"]["energy"]
                    dist_id = fission_energy_pool.add(energy_group)

                    # Nu-bar (prompt yield). As per physics doc, delayed
                    # neutrons folded into prompt.
                    yld = np.array(p0["yield"])
                    if yld.ndim == 1:
                        # Constant yield — expand to 2-row format
                        yld = np.array([[E_grid[0], E_grid[-1]],
                                        [yld[0], yld[0]]])
                    yield_id = fission_yield_pool.add(yld)

                    reaction_descriptors.append({
                        "type": FISSION,
                        "xs_offset": xs_off,
                        "xs_length": len(xs),
                        "threshold_idx": thresh,
                        "dist_id": dist_id,
                        "Q_value": float(r.attrs.get("Q_value", 0.0)),
                        "multiplicity": -1,  # signals energy-dependent nu
                        "yield_id": yield_id,
                    })

                elif 51 <= mt <= 90:
                    # Discrete inelastic
                    xs_data = r[TEMP]["xs"]
                    xs = np.array(xs_data)
                    thresh = int(xs_data.attrs.get("threshold_idx", 0))

                    xs_off = len(xs_pool)
                    xs_pool.extend(xs.tolist())

                    # Angular distribution
                    p0 = r["product_0"]
                    d0 = p0["distribution_0"]
                    angle = d0["angle"]
                    dist_id = angular_pool.add(angle)

                    reaction_descriptors.append({
                        "type": DISCRETE_INELASTIC,
                        "xs_offset": xs_off,
                        "xs_length": len(xs),
                        "threshold_idx": thresh,
                        "dist_id": dist_id,
                        "Q_value": float(r.attrs["Q_value"]),
                        "multiplicity": 1,
                    })

                elif mt == 91:
                    # Continuum inelastic (Kalbach-Mann)
                    xs_data = r[TEMP]["xs"]
                    xs = np.array(xs_data)
                    thresh = int(xs_data.attrs.get("threshold_idx", 0))

                    xs_off = len(xs_pool)
                    xs_pool.extend(xs.tolist())

                    d0 = r["product_0"]["distribution_0"]
                    dist_id = kalbach_pool.add(d0)

                    reaction_descriptors.append({
                        "type": CONTINUUM_INELASTIC,
                        "xs_offset": xs_off,
                        "xs_length": len(xs),
                        "threshold_idx": thresh,
                        "dist_id": dist_id,
                        "Q_value": float(r.attrs["Q_value"]),
                        "multiplicity": 1,
                    })

                elif mt == 16:
                    # (n,2n)
                    xs_data = r[TEMP]["xs"]
                    xs = np.array(xs_data)
                    thresh = int(xs_data.attrs.get("threshold_idx", 0))

                    xs_off = len(xs_pool)
                    xs_pool.extend(xs.tolist())

                    d0 = r["product_0"]["distribution_0"]
                    dist_id = kalbach_pool.add(d0)

                    reaction_descriptors.append({
                        "type": N2N,
                        "xs_offset": xs_off,
                        "xs_length": len(xs),
                        "threshold_idx": thresh,
                        "dist_id": dist_id,
                        "Q_value": float(r.attrs["Q_value"]),
                        "multiplicity": 2,
                    })

                elif mt == 17:
                    # (n,3n)
                    xs_data = r[TEMP]["xs"]
                    xs = np.array(xs_data)
                    thresh = int(xs_data.attrs.get("threshold_idx", 0))

                    xs_off = len(xs_pool)
                    xs_pool.extend(xs.tolist())

                    d0 = r["product_0"]["distribution_0"]
                    dist_id = kalbach_pool.add(d0)

                    reaction_descriptors.append({
                        "type": N3N,
                        "xs_offset": xs_off,
                        "xs_length": len(xs),
                        "threshold_idx": thresh,
                        "dist_id": dist_id,
                        "Q_value": float(r.attrs["Q_value"]),
                        "multiplicity": 3,
                    })

            # Now add the summed absorption channel
            if has_absorption:
                # Store only the non-zero tail? No — absorption spans full grid
                # for nuclides with radiative capture. Store full grid.
                xs_off = len(xs_pool)
                xs_pool.extend(absorption_xs.tolist())

                reaction_descriptors.append({
                    "type": ABSORPTION,
                    "xs_offset": xs_off,
                    "xs_length": grid_length,
                    "threshold_idx": 0,
                    "dist_id": -1,
                    "Q_value": 0.0,
                    "multiplicity": 0,
                })

            n_reactions = len(reaction_descriptors) - rxn_offset

            nuclide_descriptors.append({
                "grid_offset": grid_offset,
                "grid_length": grid_length,
                "A": awr,
                "rxn_offset": rxn_offset,
                "n_reactions": n_reactions,
                "zaid": zaid,
            })

            print(f"  {n_reactions} reactions, {grid_length} energy points")

    # ── Write binary ─────────────────────────────────────────────────
    OUTPUT_DIR.mkdir(exist_ok=True)
    outpath = OUTPUT_DIR / "nuclear_data.bin"

    print(f"\nWriting {outpath}...")
    print(f"  {len(nuclide_descriptors)} nuclides")
    print(f"  {len(reaction_descriptors)} reactions")
    print(f"  {len(energy_grids)} energy grid points")
    print(f"  {len(xs_pool)} XS pool points")
    print(f"  Angular pool: {angular_pool.n_dists} dists, "
          f"{len(angular_pool.mu)} mu points")
    print(f"  Fission energy pool: {fission_energy_pool.n_dists} dists, "
          f"{len(fission_energy_pool.E_out)} E_out points")
    print(f"  Kalbach pool: {kalbach_pool.n_dists} dists, "
          f"{len(kalbach_pool.E_out)} E_out points")
    print(f"  Fission yields: {len(fission_yield_pool.entries)} entries, "
          f"{len(fission_yield_pool.energies)} points")

    with open(outpath, "wb") as fp:
        def write_i32(v):
            fp.write(struct.pack("<i", v))

        def write_f64(v):
            fp.write(struct.pack("<d", v))

        def write_i32_array(arr):
            a = np.array(arr, dtype=np.int32)
            a.tofile(fp)

        def write_f64_array(arr):
            a = np.array(arr, dtype=np.float64)
            a.tofile(fp)

        # ── Header ───────────────────────────────────────────────────
        # All counts needed to know how much to allocate on load

        write_i32(FILE_MAGIC)
        write_i32(FILE_VERSION)

        write_i32(len(nuclide_descriptors))          # n_nuclides
        write_i32(len(reaction_descriptors))          # n_reactions
        write_i32(len(energy_grids))                  # n_energy_points
        write_i32(len(xs_pool))                       # n_xs_points

        # Angular pool sizes
        write_i32(angular_pool.n_dists)
        write_i32(len(angular_pool.energies))         # total incident energy pts
        write_i32(len(angular_pool.mu))               # total mu/pdf/cdf points

        # Fission energy pool sizes
        write_i32(fission_energy_pool.n_dists)
        write_i32(len(fission_energy_pool.energies))
        write_i32(len(fission_energy_pool.E_out))

        # Kalbach pool sizes
        write_i32(kalbach_pool.n_dists)
        write_i32(len(kalbach_pool.energies))
        write_i32(len(kalbach_pool.E_out))

        # Fission yield sizes
        write_i32(len(fission_yield_pool.entries))
        write_i32(len(fission_yield_pool.energies))

        # ── Nuclide descriptors ──────────────────────────────────────
        for nd in nuclide_descriptors:
            write_i32(nd["grid_offset"])
            write_i32(nd["grid_length"])
            write_f64(nd["A"])
            write_i32(nd["rxn_offset"])
            write_i32(nd["n_reactions"])
            write_i32(nd["zaid"])

        # ── Reaction descriptors ─────────────────────────────────────
        for rd in reaction_descriptors:
            write_i32(rd["type"])
            write_i32(rd["xs_offset"])
            write_i32(rd["xs_length"])
            write_i32(rd["threshold_idx"])
            write_i32(rd["dist_id"])
            write_f64(rd["Q_value"])
            write_i32(rd["multiplicity"])
            # yield_id (only meaningful for fission, -1 otherwise)
            write_i32(rd.get("yield_id", -1))

        # ── Fission yield descriptors ────────────────────────────────
        for (offset, n) in fission_yield_pool.entries:
            write_i32(offset)
            write_i32(n)

        # ── Double pools ─────────────────────────────────────────────
        write_f64_array(energy_grids)
        write_f64_array(xs_pool)

        # Angular pool
        write_f64_array(angular_pool.energies)
        write_f64_array(angular_pool.mu)
        write_f64_array(angular_pool.pdf)
        write_f64_array(angular_pool.cdf)

        # Fission energy pool
        write_f64_array(fission_energy_pool.energies)
        write_f64_array(fission_energy_pool.E_out)
        write_f64_array(fission_energy_pool.pdf)
        write_f64_array(fission_energy_pool.cdf)

        # Kalbach pool
        write_f64_array(kalbach_pool.energies)
        write_f64_array(kalbach_pool.E_out)
        write_f64_array(kalbach_pool.pdf)
        write_f64_array(kalbach_pool.cdf)
        write_f64_array(kalbach_pool.r)
        write_f64_array(kalbach_pool.a)

        # Fission yield data
        write_f64_array(fission_yield_pool.energies)
        write_f64_array(fission_yield_pool.nu_bar)

        # ── Int pools ────────────────────────────────────────────────
        write_i32_array(angular_pool.energy_offsets)
        write_i32_array(angular_pool.table_offsets)

        write_i32_array(fission_energy_pool.energy_offsets)
        write_i32_array(fission_energy_pool.table_offsets)

        write_i32_array(kalbach_pool.energy_offsets)
        write_i32_array(kalbach_pool.table_offsets)

    file_size = outpath.stat().st_size
    print(f"\nWrote {outpath} ({file_size / 1024:.1f} KB)")


if __name__ == "__main__":
    extract()
