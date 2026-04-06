#include "reaction.h"
#include "direction.h"
#include "sampling.h"
#include <cassert>
#include <cmath>

namespace {

constexpr double EV_TO_J = 1.602176634e-19;
constexpr double M_N = 1.67492749804e-27;
constexpr double EPS = 1e-16;

void normalize(double &x, double &y, double &z) {
  double n2 = x * x + y * y + z * z;
  if (n2 <= EPS) {
    x = 0.0;
    y = 0.0;
    z = 1.0;
    return;
  }
  double inv = 1.0 / std::sqrt(n2);
  x *= inv;
  y *= inv;
  z *= inv;
}

void set_neutron_from_velocity(Neutron &neutron, double vx, double vy,
                               double vz) {
  double v2 = vx * vx + vy * vy + vz * vz;
  if (v2 <= EPS) {
    neutron.alive = false;
    return;
  }
  double v = std::sqrt(v2);
  neutron.Omega_x = vx / v;
  neutron.Omega_y = vy / v;
  neutron.Omega_z = vz / v;
  neutron.E = 0.5 * M_N * v2 / EV_TO_J;
}

void sample_cm_direction_from_axis(double ax, double ay, double az,
                                   double mu_cm, RNG &rng, double &dx,
                                   double &dy, double &dz) {
  dx = ax;
  dy = ay;
  dz = az;
  normalize(dx, dy, dz);
  rotate_dir(dx, dy, dz, mu_cm, rng);
}

} // namespace

void elastic_scatter(Neutron &neutron, const NuclideDescriptor &nuc,
                     const ReactionDescriptor &rxn, const NuclearData &data,
                     double temperature, RNG &rng) {
  assert(rxn.type == RxnType::ELASTIC);
  const double A = nuc.A;

  const double v_mag = std::sqrt(2.0 * neutron.E * EV_TO_J / M_N);
  const double vx = v_mag * neutron.Omega_x;
  const double vy = v_mag * neutron.Omega_y;
  const double vz = v_mag * neutron.Omega_z;

  Velocity V{0.0, 0.0, 0.0};
  if (use_moving_target_elastic(neutron.E, A, temperature)) {
    V = sample_collision_conditioned_target_velocity(
        neutron.E, neutron.Omega_x, neutron.Omega_y, neutron.Omega_z, A,
        temperature, rng);
  }

  double gx = vx - V.x;
  double gy = vy - V.y;
  double gz = vz - V.z;
  double g = std::sqrt(gx * gx + gy * gy + gz * gz);
  if (g <= EPS)
    return;

  double mu_cm = sample_cosine(data.angular, rxn.dist_id, neutron.E, rng);

  double ghat_x = gx / g;
  double ghat_y = gy / g;
  double ghat_z = gz / g;
  double gprime_dir_x, gprime_dir_y, gprime_dir_z;
  sample_cm_direction_from_axis(ghat_x, ghat_y, ghat_z, mu_cm, rng,
                                gprime_dir_x, gprime_dir_y, gprime_dir_z);

  const double vcm_x = (vx + A * V.x) / (A + 1.0);
  const double vcm_y = (vy + A * V.y) / (A + 1.0);
  const double vcm_z = (vz + A * V.z) / (A + 1.0);

  const double scale = A / (A + 1.0);
  const double vpx = vcm_x + scale * g * gprime_dir_x;
  const double vpy = vcm_y + scale * g * gprime_dir_y;
  const double vpz = vcm_z + scale * g * gprime_dir_z;

  set_neutron_from_velocity(neutron, vpx, vpy, vpz);
}

void inelastic_scatter_disc(Neutron &neutron, const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, double temperature,
                            RNG &rng) {
  const double A = nuc.A;
  const double Q = rxn.Q_value;

  const double v_mag = std::sqrt(2.0 * neutron.E * EV_TO_J / M_N);
  const double vx = v_mag * neutron.Omega_x;
  const double vy = v_mag * neutron.Omega_y;
  const double vz = v_mag * neutron.Omega_z;

  Velocity V = sample_target_velocity(A, temperature, rng);

  const double gx = vx - V.x;
  const double gy = vy - V.y;
  const double gz = vz - V.z;
  const double g2 = gx * gx + gy * gy + gz * gz;
  if (g2 <= EPS) {
    neutron.alive = false;
    return;
  }

  const double mu_r = (A / (A + 1.0)) * M_N;
  const double E_rel_in = 0.5 * mu_r * g2;
  const double E_rel_out = E_rel_in + Q * EV_TO_J;
  if (E_rel_out <= 0.0) {
    neutron.alive = false;
    return;
  }

  const double g_out = std::sqrt(2.0 * E_rel_out / mu_r);

  double mu_cm = sample_cosine(data.angular, rxn.dist_id, neutron.E, rng);
  const double g = std::sqrt(g2);
  double ghat_x = gx / g;
  double ghat_y = gy / g;
  double ghat_z = gz / g;
  double gprime_dir_x, gprime_dir_y, gprime_dir_z;
  sample_cm_direction_from_axis(ghat_x, ghat_y, ghat_z, mu_cm, rng,
                                gprime_dir_x, gprime_dir_y, gprime_dir_z);

  const double vcm_x = (vx + A * V.x) / (A + 1.0);
  const double vcm_y = (vy + A * V.y) / (A + 1.0);
  const double vcm_z = (vz + A * V.z) / (A + 1.0);

  const double scale = A / (A + 1.0);
  const double vpx = vcm_x + scale * g_out * gprime_dir_x;
  const double vpy = vcm_y + scale * g_out * gprime_dir_y;
  const double vpz = vcm_z + scale * g_out * gprime_dir_z;

  set_neutron_from_velocity(neutron, vpx, vpy, vpz);
}

void inelastic_scatter_cont(Neutron &neutron, const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, double temperature,
                            RNG &rng) {
  const double A = nuc.A;

  const double v_mag = std::sqrt(2.0 * neutron.E * EV_TO_J / M_N);
  const double vx = v_mag * neutron.Omega_x;
  const double vy = v_mag * neutron.Omega_y;
  const double vz = v_mag * neutron.Omega_z;

  Velocity V = sample_target_velocity(A, temperature, rng);

  const double gx = vx - V.x;
  const double gy = vy - V.y;
  const double gz = vz - V.z;
  const double g = std::sqrt(gx * gx + gy * gy + gz * gz);
  if (g <= EPS) {
    neutron.alive = false;
    return;
  }

  KalbachResult result =
      sample_kalbach_mann(data.kalbach, rxn.dist_id, neutron.E, rng);

  double ghat_x = gx / g;
  double ghat_y = gy / g;
  double ghat_z = gz / g;
  double out_dir_x, out_dir_y, out_dir_z;
  sample_cm_direction_from_axis(ghat_x, ghat_y, ghat_z, result.mu, rng,
                                out_dir_x, out_dir_y, out_dir_z);

  const double vcm_x = (vx + A * V.x) / (A + 1.0);
  const double vcm_y = (vy + A * V.y) / (A + 1.0);
  const double vcm_z = (vz + A * V.z) / (A + 1.0);

  const double v_out_cm = std::sqrt(2.0 * result.E_out * EV_TO_J / M_N);
  const double vpx = vcm_x + v_out_cm * out_dir_x;
  const double vpy = vcm_y + v_out_cm * out_dir_y;
  const double vpz = vcm_z + v_out_cm * out_dir_z;

  set_neutron_from_velocity(neutron, vpx, vpy, vpz);
}

void fission(Neutron &neutron, const ReactionDescriptor &rxn,
             const NuclearData &data, ParticleBank &fission_bank, double k_eff,
             RNG &rng) {
  const int nu_prompt =
      sample_nu_bar(data.fission_yields, rxn.yield_id, neutron.E, rng);

  for (int i = 0; i < nu_prompt; ++i) {
    Neutron secondary;
    secondary.x = neutron.x;
    secondary.y = neutron.y;
    secondary.z = neutron.z;
    secondary.E =
        sample_fission_energy(data.fission_energy, rxn.dist_id, neutron.E, rng);
    secondary.w = neutron.w / k_eff;
    sample_isodir(secondary.Omega_x, secondary.Omega_y, secondary.Omega_z, rng);
    fission_bank.push_back(secondary);
  }

  if (rxn.delayed_id >= 0 && rxn.delayed_id < data.n_delayed_fission_descs) {
    const DelayedFissionDescriptor &delayed =
        data.delayed_fission_descs[rxn.delayed_id];
    for (int g = 0; g < delayed.n_groups; ++g) {
      const DelayedNeutronGroupDescriptor &group =
          data.delayed_groups[delayed.group_offset + g];
      const int nu_delayed =
          sample_nu_bar(data.fission_yields, group.yield_id, neutron.E, rng);

      for (int i = 0; i < nu_delayed; ++i) {
        Neutron secondary;
        secondary.x = neutron.x;
        secondary.y = neutron.y;
        secondary.z = neutron.z;
        secondary.E = sample_fission_energy(data.delayed_fission_energy,
                                            group.dist_id, neutron.E, rng);
        secondary.w = neutron.w / k_eff;
        sample_isodir(secondary.Omega_x, secondary.Omega_y, secondary.Omega_z,
                      rng);
        fission_bank.push_back(secondary);
      }
    }
  }

  neutron.alive = false;
}

void capture(Neutron &neutron) { neutron.alive = false; }

void multiply(Neutron &neutron, const NuclideDescriptor &nuc,
              const ReactionDescriptor &rxn, const NuclearData &data,
              double temperature, ParticleBank &secondary_bank, RNG &rng) {
  const double A = nuc.A;

  const double E_inc = neutron.E;
  const double v_mag = std::sqrt(2.0 * E_inc * EV_TO_J / M_N);
  const double vx = v_mag * neutron.Omega_x;
  const double vy = v_mag * neutron.Omega_y;
  const double vz = v_mag * neutron.Omega_z;

  Velocity V = sample_target_velocity(A, temperature, rng);

  const double gx = vx - V.x;
  const double gy = vy - V.y;
  const double gz = vz - V.z;
  const double g = std::sqrt(gx * gx + gy * gy + gz * gz);
  if (g <= EPS) {
    neutron.alive = false;
    return;
  }

  const double ghat_x = gx / g;
  const double ghat_y = gy / g;
  const double ghat_z = gz / g;

  const double vcm_x = (vx + A * V.x) / (A + 1.0);
  const double vcm_y = (vy + A * V.y) / (A + 1.0);
  const double vcm_z = (vz + A * V.z) / (A + 1.0);

  // Update original neutron with first Kalbach sample
  KalbachResult result =
      sample_kalbach_mann(data.kalbach, rxn.dist_id, E_inc, rng);
  double out_dir_x, out_dir_y, out_dir_z;
  sample_cm_direction_from_axis(ghat_x, ghat_y, ghat_z, result.mu, rng,
                                out_dir_x, out_dir_y, out_dir_z);
  {
    const double v_out_cm = std::sqrt(2.0 * result.E_out * EV_TO_J / M_N);
    const double vpx = vcm_x + v_out_cm * out_dir_x;
    const double vpy = vcm_y + v_out_cm * out_dir_y;
    const double vpz = vcm_z + v_out_cm * out_dir_z;
    set_neutron_from_velocity(neutron, vpx, vpy, vpz);
  }

  // Create multiplicity - 1 secondaries from original incident energy
  for (int i = 0; i < rxn.multiplicity - 1; ++i) {
    Neutron secondary;
    secondary.x = neutron.x;
    secondary.y = neutron.y;
    secondary.z = neutron.z;
    secondary.w = neutron.w;

    KalbachResult sec_result =
        sample_kalbach_mann(data.kalbach, rxn.dist_id, E_inc, rng);

    sample_cm_direction_from_axis(ghat_x, ghat_y, ghat_z, sec_result.mu, rng,
                                  out_dir_x, out_dir_y, out_dir_z);
    const double v_out_cm = std::sqrt(2.0 * sec_result.E_out * EV_TO_J / M_N);
    const double svx = vcm_x + v_out_cm * out_dir_x;
    const double svy = vcm_y + v_out_cm * out_dir_y;
    const double svz = vcm_z + v_out_cm * out_dir_z;
    set_neutron_from_velocity(secondary, svx, svy, svz);
    if (!secondary.alive)
      continue;

    secondary_bank.push_back(secondary);
  }
}
