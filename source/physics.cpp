#include "../include/physics.h"
#include "../include/Material.h"
#include "../include/NuclearData.h"
#include "../include/rng.h"
#include <algorithm>
#include <cmath>

namespace micromc {
inline constexpr double pi{3.14159265358979323846};

int sign(double x) { return (x > 0) - (x < 0); }
}; // namespace micromc

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

double lookup_xs(const NuclearData &data, const NuclideDescriptor &nuc,
                 const ReactionDescriptor &rxn, double E) {
  const double *E_grid = data.energy_grids + nuc.grid_offset;
  int n = nuc.grid_length;

  int i = binary_search(E_grid, n, E);
  if (i >= n - 1)
    i = n - 2;

  int j = i - rxn.threshold_idx;
  if (j < 0 || j >= rxn.xs_length - 1) {
    if (j < 0)
      return 0.0;
    return data.xs_pool[rxn.xs_offset + rxn.xs_length - 1];
  }

  double t = (E - E_grid[i]) / (E_grid[i + 1] - E_grid[i]);
  double xs_lo = data.xs_pool[rxn.xs_offset + j];
  double xs_hi = data.xs_pool[rxn.xs_offset + j + 1];
  return xs_lo + t * (xs_hi - xs_lo);
}

ReactionSample sample_reaction(const Material &mat, const NuclearData &data,
                               double E, double Sigma_t, RNG &rng) {
  double xi = uniform(rng) * Sigma_t;
  double cumulative = 0.0;

  for (int i = 0; i < mat.n_nuclides; ++i) {
    int nid = mat.nuclide_ids[i];
    const auto &nuc = data.nuclides[nid];
    double N = mat.number_densities[i];

    for (int r = 0; r < nuc.n_reactions; ++r) {
      const auto &rxn = data.reactions[nuc.rxn_offset + r];
      cumulative += N * lookup_xs(data, nuc, rxn, E);
      if (cumulative >= xi)
        return {nid, nuc.rxn_offset + r, static_cast<RxnType>(rxn.type)};
    }
  }

  // Fallback: floating-point edge case where xi == sigma_t exactly
  const auto &last_nuc = data.nuclides[mat.nuclide_ids[mat.n_nuclides - 1]];
  const auto &last_rxn =
      data.reactions[last_nuc.rxn_offset + last_nuc.n_reactions - 1];
  return {mat.nuclide_ids[mat.n_nuclides - 1],
          last_nuc.rxn_offset + last_nuc.n_reactions - 1,
          static_cast<RxnType>(last_rxn.type)};
}

// This is how we will sample our various CDFs
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

void sample_isodir(double &Omega_x, double &Omega_y, double &Omega_z,
                   RNG &rng) {
  double mu = 2 * uniform(rng) - 1;
  double varphi = 2 * micromc::pi * uniform(rng);

  double sin_theta = std::sqrt(1.0 - mu * mu);

  Omega_x = sin_theta * std::cos(varphi);
  Omega_y = sin_theta * std::sin(varphi);
  Omega_z = mu;
}

void rotate_dir(double &Omega_x, double &Omega_y, double &Omega_z, double mu,
                RNG &rng) {
  double varphi = 2.0 * micromc::pi * uniform(rng);

  mu = std::clamp(mu, -1.0, 1.0);

  double sin_theta = std::sqrt(std::max(0.0, 1.0 - mu * mu));
  double cos_varphi = std::cos(varphi);
  double sin_varphi = std::sin(varphi);

  double uz = Omega_z;
  double denom2 = std::max(0.0, 1.0 - uz * uz);

  if (denom2 < 1e-14) {
    Omega_x = sin_theta * cos_varphi;
    Omega_y = sin_theta * sin_varphi;
    Omega_z = mu * micromc::sign(uz);
  } else {
    double denom = std::sqrt(denom2);

    double Omega_prime_z = mu * uz + sin_theta * denom * cos_varphi;

    double Omega_prime_x = (Omega_x * (mu - uz * Omega_prime_z) -
                            Omega_y * denom * sin_theta * sin_varphi) /
                           denom2;

    double Omega_prime_y = (Omega_y * (mu - uz * Omega_prime_z) +
                            Omega_x * denom * sin_theta * sin_varphi) /
                           denom2;

    Omega_x = Omega_prime_x;
    Omega_y = Omega_prime_y;
    Omega_z = Omega_prime_z;
  }
  double norm =
      std::sqrt(Omega_x * Omega_x + Omega_y * Omega_y + Omega_z * Omega_z);
  Omega_x /= norm;
  Omega_y /= norm;
  Omega_z /= norm;
};

int stoch_energy_interp(const int *energy_offsets, const double *energies,
                        int dist_id, double E, RNG &rng) {
  int first = energy_offsets[dist_id];
  int last = energy_offsets[dist_id + 1];
  int n = last - first;

  if (n <= 1)
    return first;

  int i = binary_search(energies + first, n, E);
  if (i >= n - 1)
    i = n - 2;

  double E_lo = energies[first + i];
  double E_hi = energies[first + i + 1];
  double f = (E_hi > E_lo) ? (E - E_lo) / (E_hi - E_lo) : 0.0;

  return (uniform(rng) < f) ? (first + i + 1) : (first + i);
}

double sample_cosine(const AngularDistPool &pool, int dist_id, double E,
                     RNG &rng) {
  int j =
      stoch_energy_interp(pool.energy_offsets, pool.energies, dist_id, E, rng);

  int start = pool.table_offsets[j];
  int n = pool.table_offsets[j + 1] - pool.table_offsets[j];

  return sample_tab_cdf(pool.mu + start, pool.cdf + start, n, rng);
}

KalbachResult sample_kalbach_mann(const KalbachMannDistPool &pool, int dist_id,
                                  double E, RNG &rng) {
  int j =
      stoch_energy_interp(pool.energy_offsets, pool.energies, dist_id, E, rng);

  int start = pool.table_offsets[j];
  int n = pool.table_offsets[j + 1] - pool.table_offsets[j];

  // Inline CDF sampling so we keep the bin index for r/a interpolation
  double xi = uniform(rng);
  int idx = binary_search(pool.cdf + start, n, xi);
  if (idx >= n - 1)
    idx = n - 2;

  double denom = pool.cdf[start + idx + 1] - pool.cdf[start + idx];
  double t = (denom > 0.0) ? (xi - pool.cdf[start + idx]) / denom : 0.0;

  double E_out = pool.E_out[start + idx] +
                 t * (pool.E_out[start + idx + 1] - pool.E_out[start + idx]);
  double r =
      pool.r[start + idx] + t * (pool.r[start + idx + 1] - pool.r[start + idx]);
  double a =
      pool.a[start + idx] + t * (pool.a[start + idx + 1] - pool.a[start + idx]);

  // Sample mu from Kalbach-Mann
  double xi_2 = uniform(rng);
  double mu;
  if (std::abs(a) < 1e-8) {
    // Isotropic limit when a -> 0
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
  int n = pool.table_offsets[j + 1] - pool.table_offsets[j];

  return sample_tab_cdf(pool.E_out + start, pool.cdf + start, n, rng);
}

int sample_nu_bar(const FissionYieldPool &pool, int yield_id, double E,
                  RNG &rng) {
  const FissionYieldDescriptor &desc = pool.descriptors[yield_id];
  int offset = desc.offset;
  int n = desc.n_points;

  int i = binary_search(pool.energy + offset, n, E);
  if (i >= n - 1)
    i = n - 2;

  double t = (E - pool.energy[offset + i]) /
             (pool.energy[offset + i + 1] - pool.energy[offset + i]);
  double nu_bar = pool.nu_bar[offset + i] +
                  t * (pool.nu_bar[offset + i + 1] - pool.nu_bar[offset + i]);

  int nu = static_cast<int>(nu_bar);
  if (uniform(rng) < (nu_bar - nu))
    nu += 1;
  return nu;
}
