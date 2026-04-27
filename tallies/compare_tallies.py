#!/usr/bin/env python3

from __future__ import annotations

import argparse
import json
from pathlib import Path


np = None


def import_numpy():
    try:
        import numpy as numpy_module
    except ModuleNotFoundError as exc:
        raise RuntimeError("numpy is required to compare tallies") from exc

    return numpy_module


def import_pyplot():
    try:
        import matplotlib.pyplot as plt
        import matplotlib.ticker as mtick
    except ModuleNotFoundError as exc:
        raise RuntimeError(
            "matplotlib is required to plot tally comparisons"
        ) from exc

    plt.rcParams.update(
        {
            "axes.prop_cycle": plt.cycler(color=plt.cm.Set1.colors),
            "lines.linewidth": 1.5,
            "lines.linestyle": "-",
            "axes.grid": False,
            "font.size": 12,
        }
    )
    return plt, mtick


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Compare a microMC JSON tally against a Serpent detector tally."
    )
    parser.add_argument("--sfile", type=Path, required=True, help="Serpent detector file")
    parser.add_argument("--stally", required=True, help="Serpent detector name")
    parser.add_argument("--mfile", type=Path, required=True, help="microMC tally JSON file")
    parser.add_argument("--mtally", required=True, help="microMC tally name")
    parser.add_argument(
        "--out",
        type=Path,
        default=Path("compare_tally.png"),
        help="Output PNG path (default: compare_tally.png)",
    )
    parser.add_argument("--no-show", action="store_true", help="Do not show plot window")
    return parser.parse_args()


def load_micromc_tally(path: Path, tally_name: str) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    with path.open("r", encoding="utf-8") as f:
        data = json.load(f)

    selected = None
    for tally in data.get("tallies", []):
        if tally.get("name") == tally_name:
            selected = tally
            break

    if selected is None:
        raise ValueError(f"microMC tally '{tally_name}' not found in {path}")

    dims = selected.get("grid", {}).get("dims", [])
    if not dims:
        raise ValueError(f"microMC tally '{tally_name}' has no grid dims")

    edges = np.asarray(dims[0].get("bin_edges", dims[0].get("bin_edges_eV", [])), dtype=np.float64)
    values = np.asarray(selected.get("results", {}).get("mean", []), dtype=np.float64)
    std_err = np.asarray(selected.get("results", {}).get("std_err", []), dtype=np.float64)

    if edges.size != values.size + 1:
        raise ValueError(
            f"microMC grid/value mismatch for '{tally_name}': "
            f"edges={edges.size}, values={values.size}"
        )

    if std_err.size != values.size:
        std_err = np.asarray([], dtype=np.float64)

    return edges, values, std_err


def load_serpent_tally(path: Path, tally_name: str) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    try:
        import serpentTools
    except ModuleNotFoundError as exc:
        raise RuntimeError("serpentTools is required to read Serpent detector files") from exc

    det = serpentTools.read(str(path))
    if tally_name not in det.detectors:
        available = ", ".join(det.detectors.keys())
        raise KeyError(
            f"Serpent detector '{tally_name}' not found in {path}. "
            f"Available detectors: {available}"
        )

    tally = det.detectors[tally_name]
    egrid = tally.grids["E"]
    edges = np.concatenate(([egrid[0, 0]], egrid[:, 1])) * 1.0e6
    values = np.asarray(tally.tallies.ravel(), dtype=np.float64)
    rel_err = np.asarray(getattr(tally, "errors", []), dtype=np.float64).ravel()

    if edges.size != values.size + 1:
        raise ValueError(
            f"Serpent grid/value mismatch for '{tally_name}': "
            f"edges={edges.size}, values={values.size}"
        )

    if rel_err.size != values.size:
        rel_err = np.asarray([], dtype=np.float64)

    return edges, values, rel_err


def check_matching_grids(m_edges: np.ndarray, s_edges: np.ndarray) -> None:
    if m_edges.size != s_edges.size:
        raise ValueError(
            f"Energy edge count mismatch: microMC={m_edges.size}, Serpent={s_edges.size}"
        )

    edge_ok = np.allclose(m_edges, s_edges, rtol=1.0e-5, atol=1.0e-12)
    if not edge_ok:
        rel = np.abs(m_edges - s_edges) / np.maximum(np.abs(s_edges), 1.0e-30)
        idx = int(np.argmax(rel))
        raise ValueError(
            "Energy grids differ between microMC tally JSON and Serpent detector. "
            f"max_rel_diff={rel[idx]:.3e} at edge[{idx}]: "
            f"microMC={m_edges[idx]:.8e}, Serpent={s_edges[idx]:.8e}. "
            "Use matching min/max/bins and spacing."
        )


def normalize_to_unit_area(values: np.ndarray, std_err: np.ndarray) -> tuple[np.ndarray, np.ndarray]:
    total = np.sum(values)
    if not (total > 0.0) or not np.isfinite(total):
        raise ValueError("Cannot normalize tally with non-positive or non-finite sum")

    values = values / total
    if std_err.size == values.size:
        std_err = std_err / total
    return values, std_err


def plot_comparison(
    edges_eV: np.ndarray,
    micro: np.ndarray,
    micro_std: np.ndarray,
    serpent: np.ndarray,
    serpent_rel_err: np.ndarray,
    micro_label: str,
    serpent_label: str,
    out_png: Path,
    show: bool,
) -> None:
    plt, mtick = import_pyplot()
    centers = np.sqrt(edges_eV[:-1] * edges_eV[1:])

    mask_micro = micro > 0.0
    mask_serpent = serpent > 0.0
    mask_rel = mask_serpent

    rel_diff = np.full_like(serpent, np.nan, dtype=np.float64)
    rel_diff[mask_rel] = (micro[mask_rel] - serpent[mask_rel]) / serpent[mask_rel]

    combined_sigma = None
    if serpent_rel_err.size == serpent.size:
        micro_rel_err = np.zeros_like(micro)
        if micro_std.size == micro.size:
            pos = micro > 0.0
            micro_rel_err[pos] = micro_std[pos] / micro[pos]

        combined_sigma = np.full_like(serpent, np.nan, dtype=np.float64)
        finite = np.isfinite(micro_rel_err) & np.isfinite(serpent_rel_err)
        combined_sigma[finite] = np.sqrt(
            micro_rel_err[finite] * micro_rel_err[finite]
            + serpent_rel_err[finite] * serpent_rel_err[finite]
        )

    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(7, 10), sharex=True)

    ax1.stairs(micro, edges_eV, label="microMC: " + micro_label)
    ax1.stairs(serpent, edges_eV, ls="--", label="Serpent: " + serpent_label)
    ax1.set_xscale("log")
    ax1.set_ylabel(r"$\hat{t}$")
    ax1.legend()

    micro_log = np.where(mask_micro, micro, np.nan)
    serpent_log = np.where(mask_serpent, serpent, np.nan)
    ax2.stairs(micro_log, edges_eV, label="microMC" + micro_label)
    ax2.stairs(serpent_log, edges_eV, ls="--", label="Serpent: " + serpent_label)
    ax2.set_xscale("log")
    ax2.set_yscale("log")
    ax2.set_ylabel(r"$\hat{t}$")

    if combined_sigma is not None:
        band_mask = np.isfinite(combined_sigma)
        ax3.fill_between(
            centers[band_mask],
            -2.0 * combined_sigma[band_mask],
            2.0 * combined_sigma[band_mask],
            color="gray",
            alpha=0.35,
            step="mid",
            linewidth=0.0,
            label=r"Combined $\pm 2\sigma$",
        )

    ax3.stairs(rel_diff, edges_eV, linewidth=1.0, label="Relative difference")
    ax3.axhline(0.0, color="black", linewidth=0.8, alpha=0.7)
    ax3.set_xscale("log")
    ax3.set_xlabel("Energy (eV)")
    ax3.set_ylabel("Relative Difference")
    ax3.yaxis.set_major_formatter(mtick.PercentFormatter(xmax=1.0))
    ax3.legend()

    ax1.set_xlim(edges_eV[0], edges_eV[-1])
    plt.tight_layout()

    out_png.parent.mkdir(parents=True, exist_ok=True)
    plt.savefig(out_png, dpi=400)
    print(f"Saved plot: {out_png}")

    if show:
        plt.show()
    else:
        plt.close(fig)


def main() -> None:
    global np
    args = parse_args()
    np = import_numpy()

    m_edges, micro, micro_std = load_micromc_tally(args.mfile, args.mtally)
    s_edges, serpent, serpent_rel_err = load_serpent_tally(args.sfile, args.stally)

    check_matching_grids(m_edges, s_edges)

    micro, micro_std = normalize_to_unit_area(micro, micro_std)
    serpent, _ = normalize_to_unit_area(serpent, np.asarray([], dtype=np.float64))

    print(f"Loaded microMC tally '{args.mtally}' from {args.mfile}")
    print(f"Loaded Serpent detector '{args.stally}' from {args.sfile}")
    print(f"Bins: {micro.size}")
    print(f"Energy range: {m_edges[0]:.3e} - {m_edges[-1]:.3e} eV")
    print("Both tallies normalized to unit area for shape comparison.")

    plot_comparison(
        edges_eV=m_edges,
        micro=micro,
        micro_std=micro_std,
        serpent=serpent,
        serpent_rel_err=serpent_rel_err,
        micro_label=args.mtally,
        serpent_label=args.stally,
        out_png=args.out,
        show=not args.no_show,
    )


if __name__ == "__main__":
    main()
