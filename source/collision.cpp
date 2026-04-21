#include "collision.h"
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

void set_particle_from_velocity(ParticleBankView bank, int i, double vx,
                                double vy, double vz) {
  double v2 = vx * vx + vy * vy + vz * vz;
  if (v2 <= EPS) {
    bank.alive[i] = 0;
    return;
  }
  double v = std::sqrt(v2);
  bank.Omega_x[i] = vx / v;
  bank.Omega_y[i] = vy / v;
  bank.Omega_z[i] = vz / v;
  bank.E[i] = 0.5 * M_N * v2 / EV_TO_J;
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

void elastic_scatter(ParticleBankView bank, int i,
                     const NuclideDescriptor &nuc,
                     const ReactionDescriptor &rxn,
                     const NuclearData &data, double temperature) {
  assert(rxn.type == RxnType::ELASTIC);
  RNG &rng = bank.rng[i];
  const double A = nuc.A;

  const double v_mag = std::sqrt(2.0 * bank.E[i] * EV_TO_J / M_N);
  const double vx = v_mag * bank.Omega_x[i];
  const double vy = v_mag * bank.Omega_y[i];
  const double vz = v_mag * bank.Omega_z[i];

  Velocity V{0.0, 0.0, 0.0};
  if (use_moving_target_elastic(bank.E[i], A, temperature)) {
    V = sample_collision_conditioned_target_velocity(
        bank.E[i], bank.Omega_x[i], bank.Omega_y[i], bank.Omega_z[i], A,
        temperature, rng);
  }

  double gx = vx - V.x;
  double gy = vy - V.y;
  double gz = vz - V.z;
  double g = std::sqrt(gx * gx + gy * gy + gz * gz);
  if (g <= EPS)
    return;

  double mu_cm = sample_cosine(data.angular, rxn.dist_id, bank.E[i], rng);

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

  set_particle_from_velocity(bank, i, vpx, vpy, vpz);
}

void inelastic_scatter_disc(ParticleBankView bank, int i,
                            const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, double temperature) {
  RNG &rng = bank.rng[i];
  const double A = nuc.A;
  const double Q = rxn.Q_value;

  const double v_mag = std::sqrt(2.0 * bank.E[i] * EV_TO_J / M_N);
  const double vx = v_mag * bank.Omega_x[i];
  const double vy = v_mag * bank.Omega_y[i];
  const double vz = v_mag * bank.Omega_z[i];

  Velocity V = sample_target_velocity(A, temperature, rng);

  const double gx = vx - V.x;
  const double gy = vy - V.y;
  const double gz = vz - V.z;
  const double g2 = gx * gx + gy * gy + gz * gz;
  if (g2 <= EPS) {
    bank.alive[i] = 0;
    return;
  }

  const double mu_r = (A / (A + 1.0)) * M_N;
  const double E_rel_in = 0.5 * mu_r * g2;
  const double E_rel_out = E_rel_in + Q * EV_TO_J;
  if (E_rel_out <= 0.0) {
    bank.alive[i] = 0;
    return;
  }

  const double g_out = std::sqrt(2.0 * E_rel_out / mu_r);

  double mu_cm = sample_cosine(data.angular, rxn.dist_id, bank.E[i], rng);
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

  set_particle_from_velocity(bank, i, vpx, vpy, vpz);
}

void inelastic_scatter_cont(ParticleBankView bank, int i,
                            const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, double temperature) {
  RNG &rng = bank.rng[i];
  const double A = nuc.A;
  const double v_mag = std::sqrt(2.0 * bank.E[i] * EV_TO_J / M_N);
  const double vx = v_mag * bank.Omega_x[i];
  const double vy = v_mag * bank.Omega_y[i];
  const double vz = v_mag * bank.Omega_z[i];
  Velocity V = sample_target_velocity(A, temperature, rng);
  const double gx = vx - V.x;
  const double gy = vy - V.y;
  const double gz = vz - V.z;
  const double g = std::sqrt(gx * gx + gy * gy + gz * gz);
  if (g <= EPS) {
    bank.alive[i] = 0;
    return;
  }
  KalbachResult result =
      sample_kalbach_mann(data.kalbach, rxn.dist_id, bank.E[i], rng);
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
  set_particle_from_velocity(bank, i, vpx, vpy, vpz);
}

void fission(ParticleBankView bank, int i, const ReactionDescriptor &rxn,
             const NuclearData &data, ParticleBankView fission_bank,
             int &fission_count, double k_eff) {
  RNG &rng = bank.rng[i];

  auto append_secondary = [&](double E_out) {
    const int slot = fission_count;
    if (slot >= fission_bank.capacity) {
      return;
    }
    ++fission_count;

    fission_bank.x[slot] = bank.x[i];
    fission_bank.y[slot] = bank.y[i];
    fission_bank.z[slot] = bank.z[i];
    fission_bank.E[slot] = E_out;
    fission_bank.w[slot] = bank.w[i] / k_eff;
    fission_bank.macro_xs_t[slot] = 0.0;
    fission_bank.alive[slot] = 1;
    fission_bank.rxn_nuclide_idx[slot] = 0;
    fission_bank.rxn_rxn_idx[slot] = 0;
    fission_bank.rxn_type[slot] = 0;
    sample_isodir(fission_bank.Omega_x[slot], fission_bank.Omega_y[slot],
                  fission_bank.Omega_z[slot], rng);
    fission_bank.rng[slot] = RNG(rng());
  };

  const int nu_prompt =
      sample_nu_bar(data.fission_yields, rxn.yield_id, bank.E[i], rng);
  for (int n = 0; n < nu_prompt; ++n) {
    const double E_out =
        sample_fission_energy(data.fission_energy, rxn.dist_id, bank.E[i], rng);
    append_secondary(E_out);
  }

  if (rxn.delayed_id >= 0 && rxn.delayed_id < data.n_delayed_fission_descs) {
    const DelayedFissionDescriptor &delayed =
        data.delayed_fission_descs[rxn.delayed_id];
    for (int g = 0; g < delayed.n_groups; ++g) {
      const DelayedNeutronGroupDescriptor &group =
          data.delayed_groups[delayed.group_offset + g];
      const int nu_delayed =
          sample_nu_bar(data.fission_yields, group.yield_id, bank.E[i], rng);

      for (int n = 0; n < nu_delayed; ++n) {
        const double E_out = sample_fission_energy(
            data.delayed_fission_energy, group.dist_id, bank.E[i], rng);
        append_secondary(E_out);
      }
    }
  }

  bank.alive[i] = 0;
}

void multiply(ParticleBankView bank, int i, const NuclideDescriptor &nuc,
              const ReactionDescriptor &rxn, const NuclearData &data,
              double temperature, ParticleBankView secondary_bank,
              int &secondary_count) {
  RNG &rng = bank.rng[i];
  const double A = nuc.A;

  const double E_inc = bank.E[i];
  const double v_mag = std::sqrt(2.0 * E_inc * EV_TO_J / M_N);
  const double vx = v_mag * bank.Omega_x[i];
  const double vy = v_mag * bank.Omega_y[i];
  const double vz = v_mag * bank.Omega_z[i];

  Velocity V = sample_target_velocity(A, temperature, rng);

  const double gx = vx - V.x;
  const double gy = vy - V.y;
  const double gz = vz - V.z;
  const double g = std::sqrt(gx * gx + gy * gy + gz * gz);
  if (g <= EPS) {
    bank.alive[i] = 0;
    return;
  }

  const double ghat_x = gx / g;
  const double ghat_y = gy / g;
  const double ghat_z = gz / g;

  const double vcm_x = (vx + A * V.x) / (A + 1.0);
  const double vcm_y = (vy + A * V.y) / (A + 1.0);
  const double vcm_z = (vz + A * V.z) / (A + 1.0);

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
    set_particle_from_velocity(bank, i, vpx, vpy, vpz);
  }

  for (int n = 0; n < rxn.multiplicity - 1; ++n) {
    const int slot = secondary_count;
    if (slot >= secondary_bank.capacity) {
      continue;
    }
    ++secondary_count;

    secondary_bank.x[slot] = bank.x[i];
    secondary_bank.y[slot] = bank.y[i];
    secondary_bank.z[slot] = bank.z[i];
    secondary_bank.w[slot] = bank.w[i];
    secondary_bank.macro_xs_t[slot] = 0.0;
    secondary_bank.alive[slot] = 1;
    secondary_bank.rxn_nuclide_idx[slot] = 0;
    secondary_bank.rxn_rxn_idx[slot] = 0;
    secondary_bank.rxn_type[slot] = 0;

    KalbachResult sec_result =
        sample_kalbach_mann(data.kalbach, rxn.dist_id, E_inc, rng);

    sample_cm_direction_from_axis(ghat_x, ghat_y, ghat_z, sec_result.mu, rng,
                                  out_dir_x, out_dir_y, out_dir_z);
    const double v_out_cm = std::sqrt(2.0 * sec_result.E_out * EV_TO_J / M_N);
    const double svx = vcm_x + v_out_cm * out_dir_x;
    const double svy = vcm_y + v_out_cm * out_dir_y;
    const double svz = vcm_z + v_out_cm * out_dir_z;

    set_particle_from_velocity(secondary_bank, slot, svx, svy, svz);
    if (!secondary_bank.alive[slot])
      continue;

    secondary_bank.rng[slot] = RNG(rng());
  }
}
