#ifndef PHYSICS_H
#define PHYSICS_H

#include "NuclearData.h"
#include "rng.h"

struct Material;

// Returns index of largest element <= value in arr[0..n-1]
int binary_search(const double *arr, int n, double value);

// Linearly interpolated microscopic XS (barn) at energy E
double lookup_xs(const NuclearData &data, const NuclideDescriptor &nuc,
                 const ReactionDescriptor &rxn, double E);

// Result of reaction sampling
struct ReactionSample {
  int nuclide_id; // index into NuclearData::nuclides[]
  int rxn_id;     // index into NuclearData::reactions[]
  RxnType type;   // reaction type for transport dispatch
};

// Sample from a tabulated CDF: x-values and CDF of length n
double sample_tab_cdf(const double *x, const double *cdf, int n, RNG &rng);

// Sample a reaction channel proportional to macroscopic XS at energy E
ReactionSample sample_reaction(const Material &mat, const NuclearData &data,
                               double E, RNG &rng);

#endif
