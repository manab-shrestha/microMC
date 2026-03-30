#include "../include/reaction.h"
#include "../include/physics.h"
#include <cmath>

void elastic_scatter(Neutron &neutron, const NuclideDescriptor &nuc,
                     const NuclearData &data, RNG &rng) {

  double A = nuc.A;
  const ReactionDescriptor &rxn = data.reactions[nuc.rxn_offset];

  double mu_cm = sample_cosine(data.angular, rxn.dist_id, neutron.E, rng);

  double E_out = neutron.E * (A * A + 2 * A * mu_cm + 1) / ((A + 1) * (A + 1));
  double mu_lab = (A * mu_cm + 1) / std::sqrt(A * A + 2 * A * mu_cm + 1);

  rotate_dir(neutron.Omega_x, neutron.Omega_y, neutron.Omega_z, mu_lab, rng);
  neutron.E = E_out;
}

void inelastic_scatter_disc(Neutron &neutron, const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, RNG &rng) {

  double A = nuc.A;
  double Q = rxn.Q_value;

  double mu_cm = sample_cosine(data.angular, rxn.dist_id, neutron.E, rng);

  double E_lab_cm = neutron.E / (A + 1);
  double E_cm_out = neutron.E * A / (A + 1) + Q;

  if (E_cm_out <= 0.0) {
    neutron.alive = false;
    return;
  }

  double E_out =
      E_lab_cm + E_cm_out + 2.0 * mu_cm * std::sqrt(E_lab_cm * E_cm_out);
  double mu_lab =
      (std::sqrt(E_cm_out) * mu_cm + std::sqrt(E_lab_cm)) / std::sqrt(E_out);

  rotate_dir(neutron.Omega_x, neutron.Omega_y, neutron.Omega_z, mu_lab, rng);
  neutron.E = E_out;
}

void inelastic_scatter_cont(Neutron &neutron, const ReactionDescriptor &rxn,
                            const NuclearData &data, RNG &rng) {

  KalbachResult kalbach_res =
      sample_kalbach_mann(data.kalbach, rxn.dist_id, neutron.E, rng);

  rotate_dir(neutron.Omega_x, neutron.Omega_y, neutron.Omega_z, kalbach_res.mu,
             rng);
  neutron.E = kalbach_res.E_out;
}

void fission(Neutron &neutron, const ReactionDescriptor &rxn,
             const NuclearData &data, ParticleBank &fission_bank, double k_eff,
             RNG &rng) {

  const int nu =
      sample_nu_bar(data.fission_yields, rxn.yield_id, neutron.E, rng);

  for (int i = 0; i < nu; ++i) {
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

  neutron.alive = false;
}

void capture(Neutron &neutron) { neutron.alive = false; }

void multiply(Neutron &neutron, const ReactionDescriptor &rxn,
              const NuclearData &data, ParticleBank &current_bank, RNG &rng) {

  double E_inc = neutron.E; // save incident energy before modification

  // Update original neutron with first Kalbach sample
  KalbachResult kalbach_res =
      sample_kalbach_mann(data.kalbach, rxn.dist_id, E_inc, rng);
  neutron.E = kalbach_res.E_out;
  rotate_dir(neutron.Omega_x, neutron.Omega_y, neutron.Omega_z, kalbach_res.mu,
             rng);

  // Create multiplicity - 1 secondaries (sampled from original incident energy)
  for (int i = 0; i < rxn.multiplicity - 1; ++i) {
    Neutron secondary;
    secondary.x = neutron.x;
    secondary.y = neutron.y;
    secondary.z = neutron.z;
    secondary.w = neutron.w;

    KalbachResult sec_kalbach =
        sample_kalbach_mann(data.kalbach, rxn.dist_id, E_inc, rng);
    sample_isodir(secondary.Omega_x, secondary.Omega_y, secondary.Omega_z, rng);
    rotate_dir(secondary.Omega_x, secondary.Omega_y, secondary.Omega_z,
               sec_kalbach.mu, rng);
    secondary.E = sec_kalbach.E_out;
    current_bank.push_back(secondary);
  }
}
