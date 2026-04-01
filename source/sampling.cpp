#include "sampling.h"
#include "material.h"
#include "nuclear_data.h"
#include "xs_lookup.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

namespace {
constexpr double KALBACH_ISO_THRESHOLD = 1e-8;
constexpr double K_B = 1.380649e-23;
constexpr double AMU_TO_KG = 1.66053906660e-27;
constexpr double EV_TO_J = 1.602176634e-19;
constexpr double M_N = 1.67492749804e-27;
constexpr double SAFETY_FACTOR = 1.2;
constexpr double VR_SIGMA_MULT = 6.0;
constexpr int MAX_REJECTION_TRIALS = 2048;

double total_micro_xs_nuclide_at(const NuclearData &data,
                                 const NuclideDescriptor &nuc, double E) {
  GridIndex gi = find_grid_index(data, nuc, E);
  double sigma_t = 0.0;
  for (int r = 0; r < nuc.n_reactions; ++r) {
    const auto &rxn = data.reactions[nuc.rxn_offset + r];
    sigma_t += lookup_micro_xs_at(data, rxn, gi);
  }
  return sigma_t;
}

double sigma_t_max_for_nuclide(const NuclearData &data, int nuclide_idx) {
  static thread_local const NuclearData *cached_data = nullptr;
  static thread_local std::vector<double> cached_max;

  if (cached_data != &data) {
    cached_data = &data;
    cached_max.assign(data.n_nuclides, -1.0);
  }

  if (nuclide_idx < 0 || nuclide_idx >= data.n_nuclides)
    return 0.0;

  double &cached = cached_max[nuclide_idx];
  if (cached >= 0.0)
    return cached;

  const NuclideDescriptor &nuc = data.nuclides[nuclide_idx];
  const double *E_grid = data.energy_grids + nuc.grid_offset;
  double sigma_max = 0.0;
  for (int i = 0; i < nuc.grid_length; ++i)
    sigma_max =
        std::max(sigma_max, total_micro_xs_nuclide_at(data, nuc, E_grid[i]));

  cached = sigma_max;
  return cached;
}
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

  if (E <= energies[first])
    return first;
  if (E >= energies[last - 1])
    return last - 1;

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
  double r =
      pool.r[start + idx] + t * (pool.r[start + idx + 1] - pool.r[start + idx]);
  double a =
      pool.a[start + idx] + t * (pool.a[start + idx + 1] - pool.a[start + idx]);

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

  if (E <= pool.energy[offset]) {
    double nu_bar = pool.nu_bar[offset];
    int nu = static_cast<int>(nu_bar);
    if (uniform(rng) < (nu_bar - nu))
      nu += 1;
    return nu;
  }
  if (E >= pool.energy[offset + n_points - 1]) {
    double nu_bar = pool.nu_bar[offset + n_points - 1];
    int nu = static_cast<int>(nu_bar);
    if (uniform(rng) < (nu_bar - nu))
      nu += 1;
    return nu;
  }

  int bin = binary_search(pool.energy + offset, n_points, E);
  if (bin >= n_points - 1)
    bin = n_points - 2;

  double frac = (E - pool.energy[offset + bin]) /
                (pool.energy[offset + bin + 1] - pool.energy[offset + bin]);
  double nu_bar =
      pool.nu_bar[offset + bin] +
      frac * (pool.nu_bar[offset + bin + 1] - pool.nu_bar[offset + bin]);

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

ReactionChoice sample_reaction_and_macro_xs(const Material &mat,
                                            const NuclearData &data, double E,
                                            RNG &rng) {
  static thread_local std::vector<double> cumulative;
  static thread_local std::vector<ReactionSample> reaction_map;
  cumulative.clear();
  reaction_map.clear();

  double macro_xs_t = 0.0;
  for (int i = 0; i < mat.n_nuclides; ++i) {
    int nuclide_idx = mat.nuclide_ids[i];
    const auto &nuc = data.nuclides[nuclide_idx];
    double N = mat.number_densities[i];
    GridIndex gi = find_grid_index(data, nuc, E);

    for (int r = 0; r < nuc.n_reactions; ++r) {
      int rxn_idx = nuc.rxn_offset + r;
      const auto &rxn = data.reactions[rxn_idx];
      double macro = N * lookup_micro_xs_at(data, rxn, gi);
      if (macro <= 0.0)
        continue;
      macro_xs_t += macro;
      cumulative.push_back(macro_xs_t);
      reaction_map.push_back({nuclide_idx, rxn_idx, rxn.type});
    }
  }

  if (macro_xs_t <= 0.0 || cumulative.empty())
    return {{0, 0, RxnType::ABSORPTION}, 0.0, false};

  double xi = uniform(rng) * macro_xs_t;
  auto it = std::lower_bound(cumulative.begin(), cumulative.end(), xi);
  int idx = static_cast<int>(it - cumulative.begin());
  if (idx >= static_cast<int>(reaction_map.size()))
    idx = static_cast<int>(reaction_map.size()) - 1;

  return {reaction_map[idx], macro_xs_t, true};
}

Velocity sample_maxwellian_velocity(const double A, const double T, RNG &rng) {
  const double m = A * AMU_TO_KG;
  const double sigma = std::sqrt(K_B * T / m);

  std::normal_distribution<double> normal(0.0, sigma);

  return {normal(rng), normal(rng), normal(rng)};
}

Velocity sample_weighted_target_velocity(const NuclearData &data,
                                         int nuclide_idx, double vx_n,
                                         double vy_n, double vz_n,
                                         double temperature, RNG &rng) {
  const NuclideDescriptor &nuc = data.nuclides[nuclide_idx];
  const double A = nuc.A;

  const double m_t = A * AMU_TO_KG;
  const double sigma_v_t = std::sqrt(K_B * temperature / m_t);
  const double v_n = std::sqrt(vx_n * vx_n + vy_n * vy_n + vz_n * vz_n);

  const double sigma_t_max = sigma_t_max_for_nuclide(data, nuclide_idx);
  const double v_r_max = v_n + VR_SIGMA_MULT * sigma_v_t;
  const double w_max = std::max(SAFETY_FACTOR * sigma_t_max * v_r_max, 1e-30);

  for (int trial = 0; trial < MAX_REJECTION_TRIALS; ++trial) {
    Velocity V = sample_maxwellian_velocity(A, temperature, rng);
    const double gx = vx_n - V.x;
    const double gy = vy_n - V.y;
    const double gz = vz_n - V.z;
    const double v_r = std::sqrt(gx * gx + gy * gy + gz * gz);

    const double E_r = 0.5 * M_N * v_r * v_r / EV_TO_J;
    const double sigma_t_rel = total_micro_xs_nuclide_at(data, nuc, E_r);
    const double w = sigma_t_rel * v_r;

    const double p_accept = std::min(1.0, w / w_max);
    if (uniform(rng) < p_accept)
      return V;
  }

  return sample_maxwellian_velocity(A, temperature, rng);
}
