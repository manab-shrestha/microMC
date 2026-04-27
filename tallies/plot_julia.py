#!/usr/bin/env python3
"""Preview the Julia-set geometry used by micromc.

Examples:
    python tallies/plot_julia.py
    python tallies/plot_julia.py --iterations 64 --cr -0.8 --ci 0.156
    python tallies/plot_julia.py --julia-half-width 1.2 --save tallies/julia.png
"""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


def julia_escape_counts(
    x_min: float,
    x_max: float,
    y_min: float,
    y_max: float,
    nx: int,
    ny: int,
    center_x: float,
    center_y: float,
    physical_half_width: float,
    julia_half_width: float,
    cr: float,
    ci: float,
    iterations: int,
) -> np.ndarray:
    x = np.linspace(x_min, x_max, nx)
    y = np.linspace(y_min, y_max, ny)
    xx, yy = np.meshgrid(x, y, indexing="xy")

    zr = (xx - center_x) * julia_half_width / physical_half_width
    zi = (yy - center_y) * julia_half_width / physical_half_width
    counts = np.full(zr.shape, iterations, dtype=np.int32)
    active = np.ones(zr.shape, dtype=bool)

    for i in range(iterations):
        zr_active = zr[active]
        zi_active = zi[active]
        zr_next = zr_active * zr_active - zi_active * zi_active + cr
        zi_next = 2.0 * zr_active * zi_active + ci

        zr[active] = zr_next
        zi[active] = zi_next

        escaped_now = active.copy()
        escaped_now[active] = zr_next * zr_next + zi_next * zi_next > 4.0
        counts[escaped_now] = i + 1
        active &= ~escaped_now

        if not active.any():
            break

    return counts


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Plot a Julia-set preview matching GEO::inside_julia_xy."
    )
    parser.add_argument("--cr", type=float, default=-0.8, help="Real part of c")
    parser.add_argument("--ci", type=float, default=0.156, help="Imaginary part of c")
    parser.add_argument("--iterations", type=int, default=32, help="Escape iterations")
    parser.add_argument("--nx", type=int, default=1000, help="Image pixels in x")
    parser.add_argument("--ny", type=int, default=1000, help="Image pixels in y")
    parser.add_argument("--x-min", type=float, default=-5.0, help="Plot x minimum")
    parser.add_argument("--x-max", type=float, default=5.0, help="Plot x maximum")
    parser.add_argument("--y-min", type=float, default=-5.0, help="Plot y minimum")
    parser.add_argument("--y-max", type=float, default=5.0, help="Plot y maximum")
    parser.add_argument("--center-x", type=float, default=0.0, help="Geometry center x")
    parser.add_argument("--center-y", type=float, default=0.0, help="Geometry center y")
    parser.add_argument(
        "--physical-half-width",
        type=float,
        default=5.0,
        help="Physical half-width used by the C++ coordinate mapping",
    )
    parser.add_argument(
        "--julia-half-width",
        type=float,
        default=1.5,
        help="Julia-plane half-width mapped from physical-half-width",
    )
    parser.add_argument(
        "--interior-only",
        action="store_true",
        help="Draw only the pass/fail geometry mask instead of escape counts",
    )
    parser.add_argument("--save", type=Path, default=None, help="Save PNG path")
    parser.add_argument("--no-show", action="store_true", help="Do not open a window")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    if args.iterations <= 0:
        raise ValueError("--iterations must be positive")
    if args.nx <= 0 or args.ny <= 0:
        raise ValueError("--nx and --ny must be positive")
    if not args.x_max > args.x_min or not args.y_max > args.y_min:
        raise ValueError("plot max values must be greater than min values")
    if args.physical_half_width <= 0.0:
        raise ValueError("--physical-half-width must be positive")

    counts = julia_escape_counts(
        args.x_min,
        args.x_max,
        args.y_min,
        args.y_max,
        args.nx,
        args.ny,
        args.center_x,
        args.center_y,
        args.physical_half_width,
        args.julia_half_width,
        args.cr,
        args.ci,
        args.iterations,
    )

    if args.interior_only:
        image = counts == args.iterations
        cmap = "gray_r"
        colorbar_label = "inside geometry"
    else:
        image = counts
        cmap = "magma"
        colorbar_label = "escape iteration"

    fig, ax = plt.subplots(figsize=(7, 7))
    mesh = ax.imshow(
        image,
        extent=(args.x_min, args.x_max, args.y_min, args.y_max),
        origin="lower",
        cmap=cmap,
        interpolation="nearest",
    )
    fig.colorbar(mesh, ax=ax, label=colorbar_label)
    ax.axhline(0.0, color="white", linewidth=0.6, alpha=0.6)
    ax.axvline(0.0, color="white", linewidth=0.6, alpha=0.6)
    ax.set_aspect("equal", adjustable="box")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_title(
        f"Julia c={args.cr:+.6g}{args.ci:+.6g}i, "
        f"iterations={args.iterations}"
    )
    plt.tight_layout()

    if args.save is not None:
        args.save.parent.mkdir(parents=True, exist_ok=True)
        fig.savefig(args.save, dpi=300)
        print(f"Saved plot: {args.save}")

    if args.no_show:
        plt.close(fig)
    else:
        plt.show()


if __name__ == "__main__":
    main()
