#include "sampling.h"
#include "material.h"
#include "xs_lookup.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr double KALBACH_ISO_THRESHOLD = 1e-8;
} // namespace

double sample_tab_cdf(const double *x, const double *cdf, int n, RNG &rng) {
  double xi = uniform(rng);
  int idx = binary_search(cdf, n, xi);
  if (idx >= n - 1)
    idx = n - 2;

  double denom = cdf[idx + 1] - cdf[idx];
  if (denom <= 0.0)
    return x[idx];

  return x[idx] + (x[idx + 1] - x[idx]) * (xi - cdf[idx]) / denom;
}

int stoch_energy_interp(const int *energy_offsets, const double *energies,
                        int dist_id, double E, RNG &rng) {
  int first = energy_offsets[dist_id];
  int last = energy_offsets[dist_id + 1];
  int count = last - first;

  if (count <= 1)
    return first;

  int bin = binary_search(energies + first, count, E);
  if (bin >= count - 1)
    bin = count - 2;

  double E_lo = energies[first + bin];
  double E_hi = energies[first + bin + 1];
  double frac = (E_hi > E_lo) ? (E - E_lo) / (E_hi - E_lo) : 0.0;

  return (uniform(rng) < frac) ? (first + bin + 1) : (first + bin);
}

double sample_cosine(const AngularDistPool &pool, int dist_id, double E,
                     RNG &rng) {
  int j =
      stoch_energy_interp(pool.energy_offsets, pool.energies, dist_id, E, rng);

  int start = pool.table_offsets[j];
  int n_bins = pool.table_offsets[j + 1] - pool.table_offsets[j];

  return sample_tab_cdf(pool.mu + start, pool.cdf + start, n_bins, rng);
}

KalbachResult sample_kalbach_mann(const KalbachMannDistPool &pool, int dist_id,
                                  double E, RNG &rng) {
  int j =
      stoch_energy_interp(pool.energy_offsets, pool.energies, dist_id, E, rng);

  int start = pool.table_offsets[j];
  int n_bins = pool.table_offsets[j + 1] - pool.table_offsets[j];

  // Inline CDF sampling to keep the bin index for r/a interpolation
  double xi = uniform(rng);
  int idx = binary_search(pool.cdf + start, n_bins, xi);
  if (idx >= n_bins - 1)
    idx = n_bins - 2;

  double denom = pool.cdf[start + idx + 1] - pool.cdf[start + idx];
  double t = (denom > 0.0) ? (xi - pool.cdf[start + idx]) / denom : 0.0;

  double E_out = pool.E_out[start + idx] +
                 t * (pool.E_out[start + idx + 1] - pool.E_out[start + idx]);
  double r = pool.r[start + idx] +
             t * (pool.r[start + idx + 1] - pool.r[start + idx]);
  double a = pool.a[start + idx] +
             t * (pool.a[start + idx + 1] - pool.a[start + idx]);

  // Sample mu from Kalbach-Mann separability
  double xi_2 = uniform(rng);
  double mu;
  if (std::abs(a) < KALBACH_ISO_THRESHOLD) {
    mu = 2.0 * xi_2 - 1.0;
  } else {
    double B = (1 - r) * std::exp(a) - (1 + r) * std::exp(-a) -
               4 * xi_2 * std::sinh(a);
    double y = (-B + std::sqrt(B * B + 4 * (1 - r * r))) / (2 * (1 + r));
    mu = std::clamp(std::log(y) / a, -1.0, 1.0);
  }

  return {E_out, mu};
}

double sample_fission_energy(const EnergyDistPool &pool, int dist_id, double E,
                             RNG &rng) {
  int j =
      stoch_energy_interp(pool.energy_offsets, pool.energies, dist_id, E, rng);

  int start = pool.table_offsets[j];
  int n_bins = pool.table_offsets[j + 1] - pool.table_offsets[j];

  return sample_tab_cdf(pool.E_out + start, pool.cdf + start, n_bins, rng);
}

int sample_nu_bar(const FissionYieldPool &pool, int yield_id, double E,
                  RNG &rng) {
  const FissionYieldDescriptor &desc = pool.descriptors[yield_id];
  int offset = desc.offset;
  int n_points = desc.n_points;

  int bin = binary_search(pool.energy + offset, n_points, E);
  if (bin >= n_points - 1)
    bin = n_points - 2;

  double frac = (E - pool.energy[offset + bin]) /
                (pool.energy[offset + bin + 1] - pool.energy[offset + bin]);
  double nu_bar = pool.nu_bar[offset + bin] +
                  frac * (pool.nu_bar[offset + bin + 1] -
                          pool.nu_bar[offset + bin]);

  int nu = static_cast<int>(nu_bar);
  if (uniform(rng) < (nu_bar - nu))
    nu += 1;
  return nu;
}

ReactionSample sample_reaction(const Material &mat, const NuclearData &data,
                               double E, double macro_xs_t, RNG &rng) {
  double xi = uniform(rng) * macro_xs_t;
  double cumulative = 0.0;

  for (int i = 0; i < mat.n_nuclides; ++i) {
    int nuclide_idx = mat.nuclide_ids[i];
    const auto &nuc = data.nuclides[nuclide_idx];
    double N = mat.number_densities[i];
    GridIndex gi = find_grid_index(data, nuc, E);

    for (int r = 0; r < nuc.n_reactions; ++r) {
      const auto &rxn = data.reactions[nuc.rxn_offset + r];
      cumulative += N * lookup_micro_xs_at(data, rxn, gi);
      if (cumulative >= xi)
        return {nuclide_idx, nuc.rxn_offset + r, rxn.type};
    }
  }

  // Fallback: floating-point edge case where xi == macro_xs_t exactly
  const auto &last_nuc = data.nuclides[mat.nuclide_ids[mat.n_nuclides - 1]];
  const auto &last_rxn =
      data.reactions[last_nuc.rxn_offset + last_nuc.n_reactions - 1];
  return {mat.nuclide_ids[mat.n_nuclides - 1],
          last_nuc.rxn_offset + last_nuc.n_reactions - 1, last_rxn.type};
}
