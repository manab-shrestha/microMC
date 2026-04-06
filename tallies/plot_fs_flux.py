import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import serpentTools
import sys

plt.rcParams.update({
    "axes.prop_cycle": plt.cycler(color=plt.cm.Set1.colors),
    "lines.linewidth": 1.5,
    "lines.linestyle": "-",
    "axes.grid": False,
    "font.size": 12,
})

# --- Config ---
n_particles = 1000
n_active = 100

if len(sys.argv) > 1:
    n_particles = int(sys.argv[1])
if len(sys.argv) > 2:
    n_active = int(sys.argv[2])

# --- Read microMC collision tally ---
data = np.fromfile("fixed_src_flux_tally.bin", dtype=np.float64).reshape(-1, 2)
E_micromc = data[:, 0]      # assumed eV
score_micromc = data[:, 1]

print(f"Loaded {len(E_micromc)} microMC collision events")
print(f"Energy range: {E_micromc.min():.3e} - {E_micromc.max():.3e} eV")

# --- Read Serpent detector ---
det = serpentTools.read("../serpent/input_fs_det0.m")
f = det.detectors["flux_serpent"]

Egrid = f.grids["E"]
phi_serpent = f.tallies.ravel()

# --- Build Serpent energy bin edges dynamically ---
# Serpent detector energies are in MeV, convert to eV
edges_eV = np.concatenate(([Egrid[0, 0]], Egrid[:, 1])) * 1e6

n_groups = len(phi_serpent)
if len(edges_eV) != n_groups + 1:
    raise ValueError(f"Mismatch: {len(edges_eV)} edges for {n_groups} tally bins")

# --- Bin microMC onto the exact same Serpent grid ---
phi_micromc, _ = np.histogram(E_micromc, bins=edges_eV, weights=score_micromc)

# --- Normalise to unit integral for shape comparison ---
if phi_micromc.sum() > 0:
    phi_micromc = phi_micromc / phi_micromc.sum()

if phi_serpent.sum() > 0:
    phi_serpent = phi_serpent / phi_serpent.sum()

# --- Bin centres for plotting ---
bin_centres_eV = np.sqrt(edges_eV[:-1] * edges_eV[1:])

# --- Masks for log-y subplot ---
mask_micromc = phi_micromc > 0
mask_serpent = phi_serpent > 0

# --- Relative difference and Serpent uncertainty band ---
mask_rel = mask_serpent
rel_diff = np.full_like(phi_serpent, np.nan, dtype=np.float64)
rel_diff[mask_rel] = (
    phi_micromc[mask_rel] - phi_serpent[mask_rel]
) / phi_serpent[mask_rel]

two_sigma = None
if hasattr(f, "errors") and f.errors is not None:
    serpent_rel_err = f.errors.ravel()
    if serpent_rel_err.shape == phi_serpent.shape:
        two_sigma = 2.0 * serpent_rel_err

# --- Plot ---
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(7, 10), sharex=True)

# Top: linear y
ax1.step(
    bin_centres_eV,
    phi_micromc,
    where="mid",
    label="microMC"
)
ax1.step(
    bin_centres_eV,
    phi_serpent,
    where="mid",
    ls="--",
    label="Serpent"
)
ax1.set_xscale("log")
ax1.set_ylabel(r"$\phi$")
ax1.legend()

# Middle: relative difference and 2-sigma uncertainty band
if two_sigma is not None:
    ax3.fill_between(
        bin_centres_eV,
        -two_sigma,
        two_sigma,
        color="gray",
        alpha=0.35,
        step="mid",
        linewidth=0.0,
        label=r"Serpent $\pm 2\sigma$",
    )

ax3.plot(
    bin_centres_eV[mask_rel],
    rel_diff[mask_rel],
    linewidth=1.0,
)
ax3.axhline(0.0, color="black", linewidth=0.8, alpha=0.7)
ax3.set_xscale("log")
ax3.set_xlabel("Energy (eV)")
ax3.set_ylabel("Relative Difference")
ax3.yaxis.set_major_formatter(mtick.PercentFormatter(xmax=1.0))
ax3.set_ylim(-0.1, 0.1)
ax3.legend()

# Bottom: log y
ax2.step(
    bin_centres_eV[mask_micromc],
    phi_micromc[mask_micromc],
    where="mid",
    label="microMC"
)
ax2.step(
    bin_centres_eV[mask_serpent],
    phi_serpent[mask_serpent],
    where="mid",
    ls="--",
    label="Serpent"
)
ax2.set_xscale("log")
ax2.set_yscale("log")
ax2.set_ylabel(r"$\phi$")

ax1.set_xlim(edges_eV[0], edges_eV[-1])

plt.tight_layout()
plt.savefig("flux_spectra_fs.png", dpi=400)
plt.show()
