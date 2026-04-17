#include "nuclear_data.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>

const char *rxn_type_name(RxnType type) {
  switch (type) {
  case RxnType::ELASTIC:
    return "elastic";
  case RxnType::DISCRETE_INELASTIC:
    return "disc_inel";
  case RxnType::CONTINUUM_INELASTIC:
    return "cont_inel";
  case RxnType::FISSION:
    return "fission";
  case RxnType::ABSORPTION:
    return "absorption";
  case RxnType::N2N:
    return "(n,2n)";
  case RxnType::N3N:
    return "(n,3n)";
  }
  return "unknown";
}

NuclearData NuclearDataHost::view() const {
  NuclearData d{};

  d.nuclides = nuclides.data();
  d.n_nuclides = static_cast<int>(nuclides.size());
  d.reactions = reactions.data();
  d.n_reactions = static_cast<int>(reactions.size());
  d.energy_grids = energy_grids.data();
  d.n_energy_points = static_cast<int>(energy_grids.size());
  d.xs_pool = xs_pool.data();
  d.n_xs_points = static_cast<int>(xs_pool.size());

  // Angular
  d.angular.energy_offsets = ang_energy_offsets.data();
  d.angular.energies = ang_energies.data();
  d.angular.table_offsets = ang_table_offsets.data();
  d.angular.mu = ang_mu.data();
  d.angular.pdf = ang_pdf.data();
  d.angular.cdf = ang_cdf.data();
  d.angular.n_dists = static_cast<int>(ang_energy_offsets.size()) - 1;
  d.angular.total_energy_points = static_cast<int>(ang_energies.size());
  d.angular.total_mu_points = static_cast<int>(ang_mu.size());

  // Fission energy
  d.fission_energy.energy_offsets = fe_energy_offsets.data();
  d.fission_energy.energies = fe_energies.data();
  d.fission_energy.table_offsets = fe_table_offsets.data();
  d.fission_energy.E_out = fe_E_out.data();
  d.fission_energy.pdf = fe_pdf.data();
  d.fission_energy.cdf = fe_cdf.data();
  d.fission_energy.n_dists = static_cast<int>(fe_energy_offsets.size()) - 1;
  d.fission_energy.total_energy_points = static_cast<int>(fe_energies.size());
  d.fission_energy.total_out_points = static_cast<int>(fe_E_out.size());

  // Delayed fission energy
  d.delayed_fission_energy.energy_offsets = dfe_energy_offsets.data();
  d.delayed_fission_energy.energies = dfe_energies.data();
  d.delayed_fission_energy.table_offsets = dfe_table_offsets.data();
  d.delayed_fission_energy.E_out = dfe_E_out.data();
  d.delayed_fission_energy.pdf = dfe_pdf.data();
  d.delayed_fission_energy.cdf = dfe_cdf.data();
  d.delayed_fission_energy.n_dists =
      std::max(0, static_cast<int>(dfe_energy_offsets.size()) - 1);
  d.delayed_fission_energy.total_energy_points =
      static_cast<int>(dfe_energies.size());
  d.delayed_fission_energy.total_out_points =
      static_cast<int>(dfe_E_out.size());

  // Kalbach
  d.kalbach.energy_offsets = km_energy_offsets.data();
  d.kalbach.energies = km_energies.data();
  d.kalbach.table_offsets = km_table_offsets.data();
  d.kalbach.E_out = km_E_out.data();
  d.kalbach.pdf = km_pdf.data();
  d.kalbach.cdf = km_cdf.data();
  d.kalbach.r = km_r.data();
  d.kalbach.a = km_a.data();
  d.kalbach.n_dists = static_cast<int>(km_energy_offsets.size()) - 1;
  d.kalbach.total_energy_points = static_cast<int>(km_energies.size());
  d.kalbach.total_out_points = static_cast<int>(km_E_out.size());

  // Fission yields
  d.fission_yields.descriptors = fission_yield_descs.data();
  d.fission_yields.energy = fy_energy.data();
  d.fission_yields.nu_bar = fy_nu_bar.data();
  d.fission_yields.n_yields = static_cast<int>(fission_yield_descs.size());
  d.fission_yields.total_points = static_cast<int>(fy_energy.size());

  d.delayed_groups = delayed_groups.data();
  d.n_delayed_groups = static_cast<int>(delayed_groups.size());
  d.delayed_fission_descs = delayed_fission_descs.data();
  d.n_delayed_fission_descs = static_cast<int>(delayed_fission_descs.size());

  // Fission energy
  d.kappa_descs = kappa_descs.data();
  d.n_kappa_descs = static_cast<int>(kappa_descs.size());

  return d;
}

