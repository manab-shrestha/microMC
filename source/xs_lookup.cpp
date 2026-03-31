#include "xs_lookup.h"

#include <algorithm>

namespace {

double interp_frac(double E, double E_lo, double E_hi) {
  if (E_hi <= E_lo)
    return 0.0;
  return std::clamp((E - E_lo) / (E_hi - E_lo), 0.0, 1.0);
}

} // namespace

int binary_search(const double *arr, int n, double value) {
  int lo = 0, hi = n - 1;
  while (lo < hi) {
    int mid = lo + (hi - lo + 1) / 2;
    if (arr[mid] <= value)
      lo = mid;
    else
      hi = mid - 1;
  }
  return lo;
}

GridIndex find_grid_index(const NuclearData &data, const NuclideDescriptor &nuc,
                          double E) {
  const double *E_grid = data.energy_grids + nuc.grid_offset;
  int grid_len = nuc.grid_length;
  if (grid_len < 2)
    return {0, 0.0};

  if (E <= E_grid[0])
    return {0, 0.0};
  if (E >= E_grid[grid_len - 1])
    return {grid_len - 2, 1.0};

  int bin = binary_search(E_grid, grid_len, E);
  if (bin >= grid_len - 1)
    bin = grid_len - 2;
  double frac = interp_frac(E, E_grid[bin], E_grid[bin + 1]);
  return {bin, frac};
}

double lookup_micro_xs(const NuclearData &data, const NuclideDescriptor &nuc,
                       const ReactionDescriptor &rxn, double E) {
  const double *E_grid = data.energy_grids + nuc.grid_offset;
  int grid_len = nuc.grid_length;
  if (grid_len < 2)
    return 0.0;

  int bin;
  if (E <= E_grid[0]) {
    bin = 0;
  } else if (E >= E_grid[grid_len - 1]) {
    bin = grid_len - 2;
  } else {
    bin = binary_search(E_grid, grid_len, E);
    if (bin >= grid_len - 1)
      bin = grid_len - 2;
  }

  int j = bin - rxn.threshold_idx;
  if (j < 0 || j >= rxn.xs_length - 1) {
    if (j < 0)
      return 0.0;
    return data.xs_pool[rxn.xs_offset + rxn.xs_length - 1];
  }

  double frac = interp_frac(E, E_grid[bin], E_grid[bin + 1]);
  double xs_lo = data.xs_pool[rxn.xs_offset + j];
  double xs_hi = data.xs_pool[rxn.xs_offset + j + 1];
  return xs_lo + frac * (xs_hi - xs_lo);
}

double lookup_micro_xs_at(const NuclearData &data,
                          const ReactionDescriptor &rxn,
                          const GridIndex &gi) {
  int j = gi.bin - rxn.threshold_idx;
  if (j < 0)
    return 0.0;
  if (j >= rxn.xs_length - 1)
    return data.xs_pool[rxn.xs_offset + rxn.xs_length - 1];
  double xs_lo = data.xs_pool[rxn.xs_offset + j];
  double xs_hi = data.xs_pool[rxn.xs_offset + j + 1];
  double frac = std::clamp(gi.frac, 0.0, 1.0);
  return xs_lo + frac * (xs_hi - xs_lo);
}
