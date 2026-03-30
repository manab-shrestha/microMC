import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator
from cycler import cycler
import serpentTools
import sys

og_matlab_colors = [(0, 0, 1),(0, 1, 0),(1, 0, 0),(0, 1, 1),(1, 0, 1),(1, 1, 0),(0, 0, 0)]
plt.rcParams.update({"axes.prop_cycle": cycler(color=og_matlab_colors),"lines.linewidth": 1.5,"lines.linestyle": "-","axes.grid": False,"font.size": 12,})

# --- Config ---
n_particles = 1000
n_active = 100

if len(sys.argv) > 1:
    n_particles = int(sys.argv[1])
if len(sys.argv) > 2:
    n_active = int(sys.argv[2])

# --- Read microMC collision tally ---
data = np.fromfile("flux_tally.bin", dtype=np.float64).reshape(-1, 2)
E_micromc = data[:, 0]
score_micromc = data[:, 1]

print(f"Loaded {len(E_micromc)} microMC collision events")
print(f"Energy range: {E_micromc.min():.3e} – {E_micromc.max():.3e} eV")

# --- Read Serpent detector ---
det = serpentTools.read("input_det0.m")
f = det.detectors["flux_serpent"]

Egrid = f.grids["E"]
phi_serpent = f.tallies.ravel()

# Serpent energy bin edges (MeV -> eV)
edges_eV = np.r_[Egrid[:, 0], Egrid[-1, 1]] * 1e6

# --- Bin microMC onto WIMS69 grid ---
phi_micromc, _ = np.histogram(E_micromc, bins=edges_eV, weights=score_micromc)

# --- Normalise to unit integral (compare shapes) ---
phi_micromc = phi_micromc / phi_micromc.sum()
phi_serpent_norm = phi_serpent / phi_serpent.sum()

# --- Reverse so index 0 = lowest energy (group 69) ---
phi_micromc_plot = phi_micromc[::-1]
phi_serpent_plot = phi_serpent_norm[::-1]

n_groups = len(phi_serpent)
n_boundaries = n_groups + 1
boundary_indices = np.arange(n_boundaries)
boundary_labels = np.arange(n_boundaries - 1, -1, -1)  # 69, 68, ..., 1, 0

# --- Plot ---
fig, ax = plt.subplots(figsize=(12, 6))

ax.step(boundary_indices[:-1], phi_micromc_plot, where="post", ls="--", label="microMC")
ax.step(boundary_indices[:-1], phi_serpent_plot, where="post", label="Serpent")

ax.set_xlabel("WIMS69 Energy Group Boundary")
ax.set_ylabel(r"Flux per energy group, $\phi$")
ax.set_title("Neutron Flux Spectrum — microMC vs Serpent")
ax.set_xlim(0, n_boundaries - 1)

ax.set_xticks([])

ax.legend()

plt.tight_layout()
plt.savefig("flux_comparison.png", dpi=150)
plt.show()
