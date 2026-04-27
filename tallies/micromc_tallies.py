"""
parse_tallies.py

Usage guide
===========

This script parses microMC JSON tally outputs written by TallyManager and
provides:

1) run-level summary (mode, material, cycle counts, k-eff metadata)
2) list of available tallies and filters
3) optional per-tally summary
4) optional plots for tally spectra, spatial meshes, and k-eff history

Typical workflow
----------------

Run from the `tallies/` directory after a simulation:

    python parse_tallies.py --file results.json

Inspect one tally by name:

    python parse_tallies.py --file results.json --tally flux_all

Plot one tally spectrum (linear + log subplot):

    python parse_tallies.py --file results.json --tally flux_all --plot

Save tally plot without opening GUI window:

    python parse_tallies.py --file results.json --tally flux_all --plot \
        --save flux_all.png --no-show

Plot k-eff active-cycle history:

    python parse_tallies.py --file results.json --plot-k

Plot an XY mesh projection:

    python parse_tallies.py --file results.json --tally mesh_flux_all \
        --plot-mesh --plane XY --save mesh_xy.png --no-show

Plot an XZ mesh slice at Y bin 10:

    python parse_tallies.py --file results.json --tally mesh_flux_all \
        --plot-mesh --plane XZ --slice-y 10

Plot a 3D spatial mesh point cloud:

    python parse_tallies.py --file results.json --tally mesh_flux_all \
        --plot-mesh3d --save mesh_3d.png --no-show

Plot a 1D spatial grid:

    python parse_tallies.py --file results.json --tally flux_x \
        --plot1d --axis X --save flux_x.png --no-show

Plot both tally spectrum and k-history in one run:

    python parse_tallies.py --file results.json --tally flux_all \
        --plot --plot-k --save flux_all.png

    Note: when both --plot and --plot-k are used with --save, the k-history
    figure is saved as `<save_basename>_k_history.png`.

Key options
-----------

--file   Path to JSON output (default: results.json)
--tally  Tally name to inspect (must match JSON `tallies[i].name`)
--plot   Plot selected tally (requires --tally)
--plot1d Plot selected tally as a 1D grid line plot
--plot-mesh Plot selected tally as a 2D spatial mesh projection or slice
--plot-mesh3d Plot selected tally as a 3D spatial mesh point cloud
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


def _grid_dims(tally: dict) -> list[dict]:
    dims = tally.get("grid", {}).get("dims", [])
    if not dims:
        raise ValueError("Tally has no grid dims")
    return dims


def _dim_name(dim: dict) -> str:
    return str(dim.get("dim", "")).upper()


def _dim_edges(dim: dict) -> np.ndarray:
    edges = dim.get("bin_edges", dim.get("bin_edges_eV", []))
    if len(edges) < 2:
        raise ValueError(f"Grid dim {_dim_name(dim)} has fewer than 2 edges")
    return np.asarray(edges, dtype=np.float64)


def _dim_index(tally: dict, name: str) -> int:
    target = name.upper()
    for i, dim in enumerate(_grid_dims(tally)):
        if _dim_name(dim) == target:
            return i
    raise ValueError(f"Tally has no {target} grid dimension")


def _energy_edges(tally: dict) -> np.ndarray:
    return _dim_edges(_grid_dims(tally)[_dim_index(tally, "ENERGY")])


def _result_array(tally: dict, key: str = "mean") -> np.ndarray:
    shape = tally.get("grid", {}).get("grid_shape", [])
    if not shape:
        raise ValueError("Tally has no grid_shape")

    values = np.asarray(tally.get("results", {}).get(key, []), dtype=np.float64)
    expected = int(np.prod(shape))
    if values.size != expected:
        raise ValueError(
            f"Result '{key}' length {values.size} does not match grid bins {expected}"
        )
    return values.reshape(tuple(int(n) for n in shape))


def _axis_names(tally: dict) -> list[str]:
    return [_dim_name(dim) for dim in _grid_dims(tally)]


def _axis_label(name: str) -> str:
    name = name.upper()
    if name == "ENERGY":
        return r"$E$ (eV)"
    if name in {"X", "Y", "Z"}:
        return rf"${name.lower()}$ (cm)"
    return name


def _bin_centers(edges_eV: np.ndarray) -> np.ndarray:
    lo = edges_eV[:-1]
    hi = edges_eV[1:]
    with np.errstate(invalid="ignore"):
        geom = np.sqrt(lo * hi)
    bad = ~np.isfinite(geom) | (geom <= 0.0)
    if np.any(bad):
        geom[bad] = 0.5 * (lo[bad] + hi[bad])
    return geom


def _dim_centers(dim: dict) -> np.ndarray:
    edges = _dim_edges(dim)
    if _dim_name(dim) == "ENERGY":
        return _bin_centers(edges)
    return 0.5 * (edges[:-1] + edges[1:])


def print_tally_summary(tally: dict) -> None:
    name = tally.get("name", "unnamed")
    quantity = tally.get("quantity", "unknown")
    results = tally.get("results", {})
    mean = np.asarray(results.get("mean", []), dtype=np.float64)
    std_err = np.asarray(results.get("std_err", []), dtype=np.float64)
    dims = _grid_dims(tally)
    axis_desc = [
        f"{_dim_name(dim)}:{len(_dim_edges(dim)) - 1}" for dim in dims
    ]

    print(f"=== Tally: {name} ({quantity}) ===")
    print(f"grid: {' x '.join(axis_desc)}")
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

    e_axis = _dim_index(tally, "ENERGY")
    edges = _energy_edges(tally)
    centers = _bin_centers(edges)

    results = tally.get("results", {})
    mean_grid = _result_array(tally, "mean")
    reduce_axes = tuple(i for i in range(mean_grid.ndim) if i != e_axis)
    mean = np.sum(mean_grid, axis=reduce_axes) if reduce_axes else mean_grid

    std_err = np.asarray(results.get("std_err", []), dtype=np.float64)
    if std_err.size == mean_grid.size:
        std_grid = std_err.reshape(mean_grid.shape)
        std_err = (
            np.sqrt(np.sum(std_grid * std_grid, axis=reduce_axes))
            if reduce_axes
            else std_grid
        )

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
    ax2.set_xlabel(_axis_label("ENERGY"))
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


def _slice_or_reduce_to_plane(
    values: np.ndarray,
    names: list[str],
    keep: list[str],
    reduce_mode: str,
    slices: dict[str, int | None],
) -> tuple[np.ndarray, list[str]]:
    arr = values
    axes = list(names)

    for axis in reversed(range(len(axes))):
        name = axes[axis]
        idx = slices.get(name)
        if idx is None:
            continue
        if name in keep:
            raise ValueError(f"Cannot slice plotted plane dimension {name}")
        if idx < 0 or idx >= arr.shape[axis]:
            raise ValueError(
                f"Slice index for {name}={idx} is outside [0, {arr.shape[axis] - 1}]"
            )
        arr = np.take(arr, idx, axis=axis)
        axes.pop(axis)

    for axis in reversed(range(len(axes))):
        if axes[axis] in keep:
            continue
        if reduce_mode == "sum":
            arr = np.sum(arr, axis=axis)
        elif reduce_mode == "mean":
            arr = np.mean(arr, axis=axis)
        else:
            raise ValueError(f"Unsupported reduce mode: {reduce_mode}")
        axes.pop(axis)

    if axes != keep:
        order = [axes.index(name) for name in keep]
        arr = np.transpose(arr, order)
        axes = keep

    return arr, axes


def _slice_or_reduce_to_axes(
    values: np.ndarray,
    names: list[str],
    keep: list[str],
    reduce_mode: str,
    slices: dict[str, int | None],
) -> tuple[np.ndarray, list[str]]:
    arr = values
    axes = list(names)

    for axis in reversed(range(len(axes))):
        name = axes[axis]
        idx = slices.get(name)
        if idx is None:
            continue
        if name in keep:
            raise ValueError(f"Cannot slice plotted dimension {name}")
        if idx < 0 or idx >= arr.shape[axis]:
            raise ValueError(
                f"Slice index for {name}={idx} is outside [0, {arr.shape[axis] - 1}]"
            )
        arr = np.take(arr, idx, axis=axis)
        axes.pop(axis)

    for axis in reversed(range(len(axes))):
        if axes[axis] in keep:
            continue
        if reduce_mode == "sum":
            arr = np.sum(arr, axis=axis)
        elif reduce_mode == "mean":
            arr = np.mean(arr, axis=axis)
        else:
            raise ValueError(f"Unsupported reduce mode: {reduce_mode}")
        axes.pop(axis)

    if axes != keep:
        order = [axes.index(name) for name in keep]
        arr = np.transpose(arr, order)
        axes = keep

    return arr, axes


def plot_tally_1d(
    tally: dict,
    axis: str,
    reduce_mode: str,
    slices: dict[str, int | None],
    out_png: Path | None,
    show: bool,
) -> None:
    if plt is None:
        raise RuntimeError("matplotlib is not available; cannot plot")

    axis = axis.upper()
    names = _axis_names(tally)
    if axis not in names:
        raise ValueError(f"Tally has no {axis} grid dimension")

    values = _result_array(tally, "mean")
    line, _ = _slice_or_reduce_to_axes(values, names, [axis], reduce_mode, slices)

    std_err = np.asarray(tally.get("results", {}).get("std_err", []), dtype=np.float64)
    line_std = np.asarray([], dtype=np.float64)
    if std_err.size == values.size:
        std_grid = std_err.reshape(values.shape)
        line_std, _ = _slice_or_reduce_to_axes(
            std_grid * std_grid, names, [axis], "sum", slices
        )
        line_std = np.sqrt(line_std)

    dim = _grid_dims(tally)[_dim_index(tally, axis)]
    edges = _dim_edges(dim)
    centers = _dim_centers(dim)
    if line.size != centers.size:
        raise ValueError(
            f"Reduced line length {line.size} does not match {axis} bins {centers.size}"
        )

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.step(centers, line, where="mid", label=tally.get("name", "tally"))
    if line_std.size == line.size:
        ax.fill_between(
            centers,
            line - line_std,
            line + line_std,
            step="mid",
            alpha=0.35,
            label=r"$\pm 1\sigma$",
        )
    ax.set_xlabel(_axis_label(axis))
    ax.set_ylabel(r"$\hat{t}$")
    if axis == "ENERGY":
        ax.set_xscale("log")
    ax.legend()

    reduced = [name for name in names if name != axis and slices.get(name) is None]
    sliced = [f"{name}[{slices[name]}]" for name in names if slices.get(name) is not None]
    subtitle = []
    if reduced:
        subtitle.append(f"{reduce_mode} over {', '.join(reduced)}")
    if sliced:
        subtitle.append(f"sliced at {', '.join(sliced)}")
    title = f"{tally.get('name', 'tally')} {axis} profile"
    if subtitle:
        title += " (" + "; ".join(subtitle) + ")"
    #ax.set_title:(title)
    plt.tight_layout()

    if out_png is not None:
        out_png.parent.mkdir(parents=True, exist_ok=True)
        plt.savefig(out_png, dpi=300)
        print(f"Saved plot: {out_png}")

    if show:
        plt.show()
    else:
        plt.close(fig)


def plot_tally_mesh(
    tally: dict,
    plane: str,
    reduce_mode: str,
    slices: dict[str, int | None],
    out_png: Path | None,
    show: bool,
) -> None:
    if plt is None:
        raise RuntimeError("matplotlib is not available; cannot plot")

    plane = plane.upper()
    if len(plane) != 2 or any(c not in "XYZ" for c in plane) or plane[0] == plane[1]:
        raise ValueError("--plane must be one of XY, XZ, YX, YZ, ZX, ZY")

    dims = _grid_dims(tally)
    names = _axis_names(tally)
    keep = list(plane)
    for name in keep:
        if name not in names:
            raise ValueError(f"Tally has no {name} grid dimension")

    values = _result_array(tally, "mean")
    image, _ = _slice_or_reduce_to_plane(values, names, keep, reduce_mode, slices)

    edge_by_name = {_dim_name(dim): _dim_edges(dim) for dim in dims}
    x_edges = edge_by_name[keep[0]]
    y_edges = edge_by_name[keep[1]]

    fig, ax = plt.subplots(figsize=(7, 6))
    mesh = ax.pcolormesh(x_edges, y_edges, image.T, shading="auto")
    fig.colorbar(mesh, ax=ax, label=r"$\hat{t}$")
    ax.set_xlabel(_axis_label(keep[0]))
    ax.set_ylabel(_axis_label(keep[1]))
    ax.set_aspect("equal", adjustable="box")

    reduced = [name for name in names if name not in keep and slices.get(name) is None]
    sliced = [f"{name}[{slices[name]}]" for name in names if slices.get(name) is not None]
    subtitle = []
    if reduced:
        subtitle.append(f"{reduce_mode} over {', '.join(reduced)}")
    if sliced:
        subtitle.append(f"sliced at {', '.join(sliced)}")
    title = f"{tally.get('name', 'tally')} {plane} mesh"
    if subtitle:
        title += " (" + "; ".join(subtitle) + ")"
    #ax.set_title(title)
    plt.tight_layout()

    if out_png is not None:
        out_png.parent.mkdir(parents=True, exist_ok=True)
        plt.savefig(out_png, dpi=300)
        print(f"Saved plot: {out_png}")

    if show:
        plt.show()
    else:
        plt.close(fig)


def plot_tally_mesh3d(
    tally: dict,
    reduce_mode: str,
    slices: dict[str, int | None],
    threshold: float,
    max_points: int,
    out_png: Path | None,
    show: bool,
) -> None:
    if plt is None:
        raise RuntimeError("matplotlib is not available; cannot plot")

    dims = _grid_dims(tally)
    names = _axis_names(tally)
    keep = ["X", "Y", "Z"]
    for name in keep:
        if name not in names:
            raise ValueError(f"Tally has no {name} grid dimension")

    values = _result_array(tally, "mean")
    volume, _ = _slice_or_reduce_to_plane(values, names, keep, reduce_mode, slices)

    edge_by_name = {_dim_name(dim): _dim_edges(dim) for dim in dims}
    dim_by_name = {_dim_name(dim): dim for dim in dims}
    centers = {name: _dim_centers(dim_by_name[name]) for name in keep}
    X, Y, Z = np.meshgrid(centers["X"], centers["Y"], centers["Z"], indexing="ij")

    mask = np.isfinite(volume) & (volume != 0.0)
    if threshold > 0.0:
        mask &= np.abs(volume) >= threshold

    if not np.any(mask):
        raise ValueError("No mesh voxels remain after applying threshold")

    x = X[mask]
    y = Y[mask]
    z = Z[mask]
    c = volume[mask]

    if max_points > 0 and c.size > max_points:
        keep_idx = np.argpartition(np.abs(c), -max_points)[-max_points:]
        x = x[keep_idx]
        y = y[keep_idx]
        z = z[keep_idx]
        c = c[keep_idx]

    fig = plt.figure(figsize=(8, 7))
    ax = fig.add_subplot(111, projection="3d")
    scatter = ax.scatter(x, y, z, c=c, s=18, cmap="viridis", alpha=0.85)
    fig.colorbar(scatter, ax=ax, label=r"$\hat{t}$", shrink=0.75)
    ax.set_xlabel(_axis_label("X"))
    ax.set_ylabel(_axis_label("Y"))
    ax.set_zlabel(_axis_label("Z"))
    if hasattr(ax, "set_box_aspect"):
        ax.set_box_aspect(
            (
                np.ptp(edge_by_name["X"]),
                np.ptp(edge_by_name["Y"]),
                np.ptp(edge_by_name["Z"]),
            )
        )

    reduced = [name for name in names if name not in keep and slices.get(name) is None]
    sliced = [f"{name}[{slices[name]}]" for name in names if slices.get(name) is not None]
    subtitle = []
    if reduced:
        subtitle.append(f"{reduce_mode} over {', '.join(reduced)}")
    if sliced:
        subtitle.append(f"sliced at {', '.join(sliced)}")
    if max_points > 0:
        subtitle.append(f"showing up to {max_points} strongest voxels")

    title = f"{tally.get('name', 'tally')} XYZ mesh"
    if subtitle:
        title += " (" + "; ".join(subtitle) + ")"
    # ax.set_title(title)
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
        default=Path("results.json"),
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
        "--plot1d",
        action="store_true",
        help="Plot selected tally as a 1D grid line plot (requires --tally)",
    )
    parser.add_argument(
        "--plot-k",
        action="store_true",
        help="Plot k-eff active-cycle history (if present)",
    )
    parser.add_argument(
        "--plot-mesh",
        action="store_true",
        help="Plot a 2D spatial mesh view of selected tally (requires --tally)",
    )
    parser.add_argument(
        "--plot-mesh3d",
        action="store_true",
        help="Plot a 3D spatial mesh point cloud of selected tally (requires --tally)",
    )
    parser.add_argument(
        "--plane",
        type=str,
        default="XY",
        help="Spatial plane for --plot-mesh: XY, XZ, YX, YZ, ZX, or ZY",
    )
    parser.add_argument(
        "--axis",
        type=str,
        default="X",
        help="Grid axis for --plot1d, e.g. X, Y, Z, or ENERGY",
    )
    parser.add_argument(
        "--mesh-reduce",
        choices=("sum", "mean"),
        default="sum",
        help="How to reduce non-plotted, non-sliced dimensions for mesh plots",
    )
    parser.add_argument(
        "--mesh3d-threshold",
        type=float,
        default=0.0,
        help="Only show 3D mesh voxels with abs(value) at least this threshold",
    )
    parser.add_argument(
        "--mesh3d-max-points",
        type=int,
        default=20000,
        help="Maximum 3D voxels to draw; keeps strongest by abs(value), 0 disables",
    )
    parser.add_argument("--slice-x", type=int, default=None, help="Slice X bin index")
    parser.add_argument("--slice-y", type=int, default=None, help="Slice Y bin index")
    parser.add_argument("--slice-z", type=int, default=None, help="Slice Z bin index")
    parser.add_argument(
        "--slice-energy", type=int, default=None, help="Slice ENERGY bin index"
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
        if args.plot1d:
            save_path = args.save
            if args.plot and save_path is not None:
                base = save_path.with_suffix("")
                save_path = base.parent / f"{base.name}_1d.png"
            plot_tally_1d(
                tally,
                args.axis,
                args.mesh_reduce,
                {
                    "X": args.slice_x,
                    "Y": args.slice_y,
                    "Z": args.slice_z,
                    "ENERGY": args.slice_energy,
                },
                save_path,
                show=not args.no_show,
            )
        if args.plot_mesh:
            save_path = args.save
            if (args.plot or args.plot1d) and save_path is not None:
                base = save_path.with_suffix("")
                save_path = base.parent / f"{base.name}_mesh.png"
            plot_tally_mesh(
                tally,
                args.plane,
                args.mesh_reduce,
                {
                    "X": args.slice_x,
                    "Y": args.slice_y,
                    "Z": args.slice_z,
                    "ENERGY": args.slice_energy,
                },
                save_path,
                show=not args.no_show,
            )
        if args.plot_mesh3d:
            save_path = args.save
            if (args.plot or args.plot1d or args.plot_mesh) and save_path is not None:
                base = save_path.with_suffix("")
                save_path = base.parent / f"{base.name}_mesh3d.png"
            plot_tally_mesh3d(
                tally,
                args.mesh_reduce,
                {
                    "X": args.slice_x,
                    "Y": args.slice_y,
                    "Z": args.slice_z,
                    "ENERGY": args.slice_energy,
                },
                args.mesh3d_threshold,
                args.mesh3d_max_points,
                save_path,
                show=not args.no_show,
            )

    if args.plot_k:
        save_path = args.save
        if (
            (args.plot or args.plot1d or args.plot_mesh or args.plot_mesh3d)
            and args.tally is not None
            and save_path is not None
        ):
            base = save_path.with_suffix("")
            save_path = base.parent / f"{base.name}_k_history.png"
        plot_k_history(data, save_path, show=not args.no_show)


if __name__ == "__main__":
    main()
