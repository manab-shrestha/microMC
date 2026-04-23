"""
parse_tallies.py

Usage guide
===========

This script parses microMC JSON tally outputs written by TallyManager and
provides:

1) run-level summary (mode, material, cycle counts, k-eff metadata)
2) list of available tallies and filters
3) optional per-tally summary
4) optional plots for tally spectra and k-eff active-cycle history

Typical workflow
----------------

Run from the `tallies/` directory after a simulation:

    python parse_tallies.py --file k_eigenvalue_tallies.json

Inspect one tally by name:

    python parse_tallies.py --file k_eigenvalue_tallies.json --tally flux_all

Plot one tally spectrum (linear + log subplot):

    python parse_tallies.py --file k_eigenvalue_tallies.json --tally flux_all --plot

Save tally plot without opening GUI window:

    python parse_tallies.py --file k_eigenvalue_tallies.json --tally flux_all --plot \
        --save flux_all.png --no-show

Plot k-eff active-cycle history:

    python parse_tallies.py --file k_eigenvalue_tallies.json --plot-k

Plot both tally spectrum and k-history in one run:

    python parse_tallies.py --file k_eigenvalue_tallies.json --tally flux_all \
        --plot --plot-k --save flux_all.png

    Note: when both --plot and --plot-k are used with --save, the k-history
    figure is saved as `<save_basename>_k_history.png`.

Key options
-----------

--file   Path to JSON output (default: k_eigenvalue_tallies.json)
--tally  Tally name to inspect (must match JSON `tallies[i].name`)
--plot   Plot selected tally (requires --tally)
--plot-k Plot k-eff active-cycle history if present in run_metadata
--save   Save figure to PNG path
--no-show  Disable interactive plot windows (for batch/remote usage)

Dependencies
------------

- Required: numpy
- Optional for plotting: matplotlib

If matplotlib is not available, text parsing still works and plot actions will
raise a clear runtime error.
"""

import argparse
import json
from pathlib import Path

import numpy as np

try:
    import matplotlib.pyplot as plt
except Exception:
    plt = None


def load_tally_json(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as f:
        return json.load(f)


def print_run_summary(data: dict) -> None:
    meta = data.get("run_metadata", {})
    print("=== Run Metadata ===")
    print(f"mode: {meta.get('mode', 'unknown')}")
    print(f"material: {meta.get('material_name', 'unknown')}")
    print(f"temperature_K: {meta.get('temperature_K', 'unknown')}")
    print(f"n_particles: {meta.get('n_particles', 'unknown')}")
    print(f"n_inactive: {meta.get('n_inactive', 'unknown')}")
    print(f"n_active: {meta.get('n_active', 'unknown')}")
    print(f"seed: {meta.get('seed', 'unknown')}")

    if "k_eff_final_mean" in meta:
        k_mean = meta.get("k_eff_final_mean", 0.0)
        k_rel = meta.get("k_eff_final_rel_err", 0.0)
        print(f"k_eff_final_mean: {k_mean:.8f}")
        print(f"k_eff_final_rel_err: {k_rel:.6e}")

    hist = meta.get("k_eff_active_history", [])
    if hist:
        print(f"k_eff_active_cycles_recorded: {len(hist)}")
    print()


def print_tally_list(data: dict) -> None:
    tallies = data.get("tallies", [])
    print("=== Tallies ===")
    if not tallies:
        print("No tallies found")
        print()
        return

    for tally in tallies:
        name = tally.get("name", "unnamed")
        quantity = tally.get("quantity", "unknown")
        grid = tally.get("grid", {})
        shape = grid.get("grid_shape", [])
        bins = int(np.prod(shape)) if shape else 0
        nuclides = tally.get("nuclide_filter_zaids", [])
        materials = tally.get("material_filter_names", [])
        print(
            f"- {name:24s} quantity={quantity:13s} bins={bins:5d} "
            f"nuclides={nuclides if nuclides else 'ALL'} "
            f"materials={materials if materials else 'ALL'}"
        )
    print()


def find_tally(data: dict, name: str) -> dict:
    for tally in data.get("tallies", []):
        if tally.get("name") == name:
            return tally
    raise ValueError(f"Tally '{name}' not found")


def _energy_edges(tally: dict) -> np.ndarray:
    dims = tally.get("grid", {}).get("dims", [])
    if not dims:
        raise ValueError("Tally has no grid dims")
    edges = dims[0].get("bin_edges_eV", [])
    if len(edges) < 2:
        raise ValueError("Energy grid has fewer than 2 edges")
    return np.asarray(edges, dtype=np.float64)


def _bin_centers(edges_eV: np.ndarray) -> np.ndarray:
    lo = edges_eV[:-1]
    hi = edges_eV[1:]
    with np.errstate(invalid="ignore"):
        geom = np.sqrt(lo * hi)
    bad = ~np.isfinite(geom) | (geom <= 0.0)
    if np.any(bad):
        geom[bad] = 0.5 * (lo[bad] + hi[bad])
    return geom


def print_tally_summary(tally: dict) -> None:
    name = tally.get("name", "unnamed")
    quantity = tally.get("quantity", "unknown")
    results = tally.get("results", {})
    mean = np.asarray(results.get("mean", []), dtype=np.float64)
    std_err = np.asarray(results.get("std_err", []), dtype=np.float64)

    print(f"=== Tally: {name} ({quantity}) ===")
    print(f"n_bins: {mean.size}")
    if mean.size:
        print(f"mean sum: {np.sum(mean):.6e}")
        print(f"mean max: {np.max(mean):.6e}")
        print(f"nonzero bins: {np.count_nonzero(mean)}")
    if std_err.size:
        finite = std_err[np.isfinite(std_err)]
        if finite.size:
            print(f"std_err max: {np.max(finite):.6e}")
    print()


def plot_tally_spectrum(tally: dict, out_png: Path | None, show: bool) -> None:
    if plt is None:
        raise RuntimeError("matplotlib is not available; cannot plot")

    edges = _energy_edges(tally)
    centers = _bin_centers(edges)

    results = tally.get("results", {})
    mean = np.asarray(results.get("mean", []), dtype=np.float64)
    std_err = np.asarray(results.get("std_err", []), dtype=np.float64)

    if mean.size != centers.size:
        raise ValueError(
            f"Mean array length {mean.size} does not match bins {centers.size}"
        )

    # Keep style aligned with existing plot_flux.py scripts.
    plt.rcParams.update(
        {
            "axes.prop_cycle": plt.cycler(color=plt.cm.Set1.colors),
            "lines.linewidth": 1.5,
            "lines.linestyle": "-",
            "axes.grid": False,
            "font.size": 12,
        }
    )

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 8), sharex=True)

    # Top: linear y spectrum
    tally_name = tally.get("name", "tally")
    ax1.step(centers, mean, where="mid", label=tally_name)
    if std_err.size == mean.size:
        lo = np.maximum(mean - std_err, 0.0)
        hi = mean + std_err
        ax1.fill_between(centers, lo, hi, step="mid", alpha=0.35, label="±1σ")
    ax1.set_xscale("log")
    ax1.set_ylabel(r"$\hat{t}$")
    ax1.legend()

    # Bottom: log y spectrum
    mask = mean > 0.0
    if np.any(mask):
        ax2.step(centers[mask], mean[mask], where="mid", label=tally_name)
    ax2.set_xscale("log")
    ax2.set_yscale("log")
    ax2.set_xlabel("Energy (eV)")
    ax2.set_ylabel(r"$\hat{t}$")

    #fig.suptitle(f"{tally.get('name', 'tally')} [{tally.get('quantity', 'unknown')}]")
    plt.tight_layout()

    if out_png is not None:
        out_png.parent.mkdir(parents=True, exist_ok=True)
        plt.savefig(out_png, dpi=300)
        print(f"Saved plot: {out_png}")

    if show:
        plt.show()
    else:
        plt.close(fig)


def plot_k_history(data: dict, out_png: Path | None, show: bool) -> None:
    if plt is None:
        raise RuntimeError("matplotlib is not available; cannot plot")

    meta = data.get("run_metadata", {})
    k_hist = np.asarray(meta.get("k_eff_active_history", []), dtype=np.float64)
    rel_hist = np.asarray(meta.get("k_eff_active_rel_err_history", []), dtype=np.float64)
    if k_hist.size == 0:
        raise ValueError("No k_eff_active_history available in this file")

    cycles = np.arange(1, k_hist.size + 1)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 8), sharex=True)
    ax1.plot(cycles, k_hist)
    ax1.set_ylabel("k_eff")
    #ax1.set_title("k-eff active-cycle history")

    if rel_hist.size == k_hist.size:
        ax2.plot(cycles, rel_hist)
    else:
        ax2.plot(cycles, np.full_like(cycles, np.nan, dtype=np.float64))
    ax2.set_xlabel("Active Cycle")
    ax2.set_ylabel("Relative Error")

    plt.tight_layout()

    if out_png is not None:
        out_png.parent.mkdir(parents=True, exist_ok=True)
        plt.savefig(out_png, dpi=300)
        print(f"Saved plot: {out_png}")

    if show:
        plt.show()
    else:
        plt.close(fig)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Parse and inspect microMC tally JSON outputs"
    )
    parser.add_argument(
        "--file",
        type=Path,
        default=Path("k_eigenvalue_tallies.json"),
        help="Path to tally JSON file",
    )
    parser.add_argument(
        "--tally",
        type=str,
        default=None,
        help="Specific tally name to inspect",
    )
    parser.add_argument(
        "--plot",
        action="store_true",
        help="Plot selected tally (requires --tally)",
    )
    parser.add_argument(
        "--plot-k",
        action="store_true",
        help="Plot k-eff active-cycle history (if present)",
    )
    parser.add_argument(
        "--save",
        type=Path,
        default=None,
        help="Save plot PNG to this path",
    )
    parser.add_argument(
        "--no-show",
        action="store_true",
        help="Do not show interactive plot windows",
    )
    args = parser.parse_args()

    data = load_tally_json(args.file)
    print_run_summary(data)
    print_tally_list(data)

    if args.tally is not None:
        tally = find_tally(data, args.tally)
        print_tally_summary(tally)
        if args.plot:
            plot_tally_spectrum(tally, args.save, show=not args.no_show)

    if args.plot_k:
        save_path = args.save
        if args.plot and args.tally is not None and save_path is not None:
            base = save_path.with_suffix("")
            save_path = base.parent / f"{base.name}_k_history.png"
        plot_k_history(data, save_path, show=not args.no_show)


if __name__ == "__main__":
    main()
