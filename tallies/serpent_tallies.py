#!/usr/bin/env python3

import argparse
from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt
from cycler import cycler
import serpentTools
from serpentTools.settings import rc

rc["serpentVersion"] = "2.1.32"

og_matlab_colors = [
    (0, 0, 1),
    (0, 1, 0),
    (1, 0, 0),
    (0, 1, 1),
    (1, 0, 1),
    (1, 1, 0),
    (0, 0, 0),
]

plt.rcParams.update(
    {
        "axes.prop_cycle": cycler(color=og_matlab_colors),
        "lines.linewidth": 1.5,
        "lines.linestyle": "-",
        "axes.grid": False,
        "font.size": 12,
    }
)


def plot_detector_spectrum(
    det_file: Path,
    det_name: str,
    out_png: Path | None = None,
    show: bool = True,
) -> None:
    det = serpentTools.read(str(det_file))

    if det_name not in det.detectors:
        available = ", ".join(det.detectors.keys())
        raise KeyError(
            f"Detector '{det_name}' not found in {det_file}.\n"
            f"Available detectors: {available}"
        )

    d = det.detectors[det_name]

    E = d.grids["E"] * 1e6
    edges = np.r_[E[:, 0], E[-1, 1]]
    centers = np.sqrt(edges[:-1] * edges[1:])

    mean = np.asarray(d.tallies.ravel(), dtype=np.float64)

    std_err = np.asarray(getattr(d, "errors", []), dtype=np.float64)
    if std_err.size != mean.size:
        std_err = np.array([], dtype=np.float64)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 8), sharex=True)

# Top: linear y
    ax1.step(centers, mean, where="mid", label=det_name)
    if std_err.size == mean.size:
        abs_err = mean * std_err
        lo = np.maximum(mean - abs_err, 0.0)
        hi = mean + abs_err
        ax1.fill_between(centers, lo, hi, step="mid", alpha=0.35, label="±1σ")
    ax1.set_xscale("log")
    ax1.set_ylabel(r"$\hat{t}$")
    ax1.legend()

# Bottom: log y
    mask = mean > 0.0
    if np.any(mask):
        ax2.step(centers[mask], mean[mask], where="mid", label=det_name)
    if std_err.size == mean.size and np.any(mask):
        abs_err = mean[mask] * std_err[mask]
        lo = np.maximum(mean[mask] - abs_err, 1e-300)
        hi = mean[mask] + abs_err
        ax2.fill_between(centers[mask], lo, hi, step="mid", alpha=0.35, label="±1σ")
    ax2.set_xscale("log")
    ax2.set_yscale("log")
    ax2.set_xlabel("Energy (eV)")
    ax2.set_ylabel(r"$\hat{t}$")
    ax2.legend()
    
    plt.tight_layout()

    if out_png is not None:
        out_png.parent.mkdir(parents=True, exist_ok=True)
        plt.savefig(out_png, dpi=300)
        print(f"Saved plot: {out_png}")

    if show:
        plt.show()
    else:
        plt.close(fig)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Plot a Serpent detector spectrum from a detector file."
    )
    parser.add_argument(
        "--file",
        type=Path,
        required=True,
        help="Path to the Serpent detector file, e.g. input_k_det0.m",
    )
    parser.add_argument(
        "--tally",
        type=str,
        required=True,
        help="Name of the detector/tally, e.g. delastic_o",
    )
    parser.add_argument(
        "--out",
        type=Path,
        default=None,
        help="Optional output PNG path",
    )
    parser.add_argument(
        "--no-show",
        action="store_true",
        help="Do not display the plot window",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    plot_detector_spectrum(
        det_file=args.file,
        det_name=args.tally,
        out_png=args.out,
        show=not args.no_show,
    )


if __name__ == "__main__":
    main()
