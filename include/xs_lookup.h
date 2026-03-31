#pragma once

#include "nuclear_data.h"

struct GridIndex {
  int bin;
  double frac; // interpolation fraction within bin
};

// Returns index of largest element <= value in arr[0..n-1]
int binary_search(const double *arr, int n, double value);

// Find the grid bin and interpolation fraction for a nuclide at energy E
GridIndex find_grid_index(const NuclearData &data,
                          const NuclideDescriptor &nuc, double E);

// Microscopic XS (barn) at energy E via binary search on the nuclide grid
double lookup_micro_xs(const NuclearData &data, const NuclideDescriptor &nuc,
                       const ReactionDescriptor &rxn, double E);

// Microscopic XS (barn) using a pre-computed grid index (no binary search)
double lookup_micro_xs_at(const NuclearData &data,
                          const ReactionDescriptor &rxn,
                          const GridIndex &gi);
