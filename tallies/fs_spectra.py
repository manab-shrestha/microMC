import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import serpentTools
import sys
import json

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
tally_name = "flux_all"

if len(sys.argv) > 1:
    n_particles = int(sys.argv[1])
if len(sys.argv) > 2:
    n_active = int(sys.argv[2])
if len(sys.argv) > 3:
    tally_name = sys.argv[3]

# --- Read microMC JSON tally ---
with open("fixed_source_tallies.json", "r", encoding="utf-8") as f_json:
    tally_data = json.load(f_json)

selected = None
for t in tally_data.get("tallies", []):
    if t.get("name") == tally_name:
        selected = t
        break

if selected is None:
    raise ValueError(f"Tally '{tally_name}' not found in fixed_source_tallies.json")

if selected.get("quantity") != "FLUX":
    raise ValueError(
        f"Tally '{tally_name}' has quantity '{selected.get('quantity')}', expected 'FLUX'"
    )

micromc_edges_eV = np.asarray(
    selected["grid"]["dims"][0]["bin_edges_eV"], dtype=np.float64
)
phi_micromc = np.asarray(selected["results"]["mean"], dtype=np.float64)
std_micromc = np.asarray(selected["results"].get("std_err", []), dtype=np.float64)

print(f"Loaded microMC tally '{tally_name}' from fixed_source_tallies.json")
print(f"microMC bins: {len(phi_micromc)}")
print(f"microMC energy range: {micromc_edges_eV[0]:.3e} - {micromc_edges_eV[-1]:.3e} eV")

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

# --- Require exact bin compatibility (no rebinning) ---
if len(micromc_edges_eV) != len(edges_eV):
    raise ValueError(
        f"Energy edge count mismatch: microMC={len(micromc_edges_eV)} Serpent={len(edges_eV)}"
    )
if len(phi_micromc) != len(phi_serpent):
    raise ValueError(
        f"Bin count mismatch: microMC={len(phi_micromc)} Serpent={len(phi_serpent)}"
    )
# Serpent detector edges in .m outputs are rounded, so allow tiny relative
# tolerance while still enforcing same bin structure (no rebin path).
edge_ok = np.allclose(micromc_edges_eV, edges_eV, rtol=1.0e-5, atol=1.0e-12)
if not edge_ok:
    rel = np.abs(micromc_edges_eV - edges_eV) / np.maximum(np.abs(edges_eV), 1.0e-30)
    idx = int(np.argmax(rel))
    raise ValueError(
        "Energy grids differ between microMC tally JSON and Serpent detector. "
        f"max_rel_diff={rel[idx]:.3e} at edge[{idx}]: "
        f"microMC={micromc_edges_eV[idx]:.8e}, Serpent={edges_eV[idx]:.8e}. "
        "Use matching min/max/bins and spacing."
    )

print("Grid compatibility check passed (exact bin match).")

# --- Normalise to unit integral for shape comparison ---
micromc_norm = phi_micromc.sum()
if micromc_norm > 0:
    phi_micromc = phi_micromc / micromc_norm
    if std_micromc.shape == phi_micromc.shape:
        std_micromc = std_micromc / micromc_norm
else:
    std_micromc = np.asarray([], dtype=np.float64)

if phi_serpent.sum() > 0:
    phi_serpent = phi_serpent / phi_serpent.sum()

# --- Bin centres for plotting ---
bin_centres_eV = np.sqrt(edges_eV[:-1] * edges_eV[1:])

# --- Masks for log-y subplot ---
mask_micromc = phi_micromc > 0
mask_serpent = phi_serpent > 0

# --- Relative difference and combined uncertainty band ---
mask_rel = mask_serpent
rel_diff = np.full_like(phi_serpent, np.nan, dtype=np.float64)
rel_diff[mask_rel] = (
    phi_micromc[mask_rel] - phi_serpent[mask_rel]
) / phi_serpent[mask_rel]

combined_sigma = None
if hasattr(f, "errors") and f.errors is not None:
    serpent_rel_err = f.errors.ravel()
    if serpent_rel_err.shape == phi_serpent.shape:
        micro_rel_err = np.full_like(phi_micromc, np.nan, dtype=np.float64)
        if std_micromc.shape == phi_micromc.shape:
            pos = phi_micromc > 0.0
            micro_rel_err[pos] = std_micromc[pos] / phi_micromc[pos]
        else:
            micro_rel_err[:] = 0.0

        combined_sigma = np.full_like(phi_serpent, np.nan, dtype=np.float64)
        finite = np.isfinite(micro_rel_err) & np.isfinite(serpent_rel_err)
        combined_sigma[finite] = np.sqrt(
            micro_rel_err[finite] * micro_rel_err[finite]
            + serpent_rel_err[finite] * serpent_rel_err[finite]
        )

# --- Plot ---
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(7, 10), sharex=True)

# Top: linear y (plot over bin edges)
ax1.stairs(phi_micromc, edges_eV, label="microMC")
ax1.stairs(phi_serpent, edges_eV, ls="--", label="Serpent")
ax1.set_xscale("log")
ax1.set_ylabel(r"$\phi$")
ax1.legend()

# Middle: relative difference and combined uncertainty band
if combined_sigma is not None:
    band_mask = np.isfinite(combined_sigma)
    ax3.fill_between(
        bin_centres_eV[band_mask],
        -2.0 * combined_sigma[band_mask],
        2.0 * combined_sigma[band_mask],
        color="gray",
        alpha=0.35,
        step="mid",
        linewidth=0.0,
        label=r"Combined $\pm 2\sigma$",
    )

rel_diff_plot = np.where(mask_rel, rel_diff, np.nan)
ax3.stairs(rel_diff_plot, edges_eV, linewidth=1.0, label="Relative difference")
ax3.axhline(0.0, color="black", linewidth=0.8, alpha=0.7)
ax3.set_xscale("log")
ax3.set_xlabel("Energy (eV)")
ax3.set_ylabel("Relative Difference")
ax3.yaxis.set_major_formatter(mtick.PercentFormatter(xmax=1.0))
ax3.set_ylim(-0.1, 0.1)
ax3.legend()

# Bottom: log y (plot over bin edges)
phi_micromc_log = np.where(mask_micromc, phi_micromc, np.nan)
phi_serpent_log = np.where(mask_serpent, phi_serpent, np.nan)
ax2.stairs(phi_micromc_log, edges_eV, label="microMC")
ax2.stairs(phi_serpent_log, edges_eV, ls="--", label="Serpent")
ax2.set_xscale("log")
ax2.set_yscale("log")
ax2.set_ylabel(r"$\phi$")

ax1.set_xlim(edges_eV[0], edges_eV[-1])

plt.tight_layout()
plt.savefig("flux_spectra_fs.png", dpi=400)
plt.show()
