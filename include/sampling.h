#pragma once

#include "nuclear_data.h"
#include "rng.h"

struct Material;

// Sample from a tabulated CDF with linear interpolation within bins
double sample_tab_cdf(const double *x, const double *cdf, int n, RNG &rng);

// Stochastic interpolation between two neighbouring energy tables
int stoch_energy_interp(const int *energy_offsets, const double *energies,
                        int dist_id, double E, RNG &rng);

// Sample scattering cosine from an angular distribution
double sample_cosine(const AngularDistPool &pool, int dist_id, double E,
                     RNG &rng);

struct KalbachResult {
  double E_out;
  double mu;
};

// Sample outgoing energy and scattering cosine from a Kalbach-Mann distribution
KalbachResult sample_kalbach_mann(const KalbachMannDistPool &pool, int dist_id,
                                  double E, RNG &rng);

// Sample outgoing energy from a fission energy distribution
double sample_fission_energy(const EnergyDistPool &pool, int dist_id, double E,
                             RNG &rng);

// Sample integer nu from nu-bar via linear interpolation + stochastic rounding
int sample_nu_bar(const FissionYieldPool &pool, int yield_id, double E,
                  RNG &rng);

// Sample a reaction channel proportional to macroscopic XS at energy E
ReactionSample sample_reaction(const Material &mat, const NuclearData &data,
                               double E, double macro_xs_t, RNG &rng);
