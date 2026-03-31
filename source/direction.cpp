#include "direction.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr double PI = 3.14159265358979323846;
constexpr double POLAR_THRESHOLD = 1e-14;

int sign(double x) { return (x > 0) - (x < 0); }
} // namespace

void sample_isodir(double &Omega_x, double &Omega_y, double &Omega_z,
                   RNG &rng) {
  double mu = 2.0 * uniform(rng) - 1.0;
  double varphi = 2.0 * PI * uniform(rng);

  double sin_theta = std::sqrt(1.0 - mu * mu);

  Omega_x = sin_theta * std::cos(varphi);
  Omega_y = sin_theta * std::sin(varphi);
  Omega_z = mu;
}

void rotate_dir(double &Omega_x, double &Omega_y, double &Omega_z, double mu,
                RNG &rng) {
  double varphi = 2.0 * PI * uniform(rng);

  mu = std::clamp(mu, -1.0, 1.0);

  double sin_theta = std::sqrt(std::max(0.0, 1.0 - mu * mu));
  double cos_varphi = std::cos(varphi);
  double sin_varphi = std::sin(varphi);

  double uz = Omega_z;
  double denom2 = std::max(0.0, 1.0 - uz * uz);

  if (denom2 < POLAR_THRESHOLD) {
    Omega_x = sin_theta * cos_varphi;
    Omega_y = sin_theta * sin_varphi;
    Omega_z = mu * sign(uz);
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
}
