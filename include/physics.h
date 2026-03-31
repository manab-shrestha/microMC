#ifndef PHYSICS_H
#define PHYSICS_H

#include "NuclearData.h"
#include "rng.h"

struct Material;

struct GridIndex {
  int i;
  double t;
};

GridIndex find_grid_index(const NuclearData &data, const NuclideDescriptor &nuc,
                          double E);

// Returns index of largest element <= value in arr[0..n-1]
int binary_search(const double *arr, int n, double value);

// Linearly interpolated microscopic XS (barn) at energy E
double lookup_xs(const NuclearData &data, const NuclideDescriptor &nuc,
                 const ReactionDescriptor &rxn, double E);

double lookup_xs_improved(const NuclearData &data,
                          const ReactionDescriptor &rxn, GridIndex &gi);

// Result of reaction sampling
struct ReactionSample {
  int nuclide_id; // index into NuclearData::nuclides[]
  int rxn_id;     // index into NuclearData::reactions[]
  RxnType type;   // reaction type for transport dispatch
};

void sample_isodir(double &Omega_x, double &Omega_y, double &Omega_z, RNG &rng);

void rotate_dir(double &Omega_x, double &Omega_y, double &Omega_z, double mu,
                RNG &rng);

// Sample from a tabulated CDF: x-values and CDF of length n
double sample_tab_cdf(const double *x, const double *cdf, int n, RNG &rng);

// Sample a reaction channel proportional to macroscopic XS at energy E
ReactionSample sample_reaction(const Material &mat, const NuclearData &data,
                               double E, double Sigma_t, RNG &rng);

struct KalbachResult {
  double E_out;
  double mu;
};

int stoch_energy_interp(const int *energy_offsets, const double *energies,
                        int dist_id, double E, RNG &rng);

double sample_cosine(const AngularDistPool &pool, int dist_id, double E,
                     RNG &rng);

// Sample outgoing energy and scattering cosine from a Kalbach-Mann distribution
KalbachResult sample_kalbach_mann(const KalbachMannDistPool &pool, int dist_id,
                                  double E, RNG &rng);

// Sample outgoing energy from a fission energy distribution
double sample_fission_energy(const EnergyDistPool &pool, int dist_id, double E,
                             RNG &rng);

// Sample integer nu from nu-bar via linear interpolation + stochastic rounding
int sample_nu_bar(const FissionYieldPool &pool, int yield_id, double E,
                  RNG &rng);

#endif
