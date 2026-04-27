#pragma once

#include "particle_bank.h"
#include "tally.h"

#include <cctype>
#include <string>
#include <vector>

inline std::string lower_ascii(std::string s) {
  for (char &c : s)
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  return s;
}

inline std::vector<TallySpec> default_tally_specs() {
  GridDimSpec energy;
  energy.dim = GridDim::ENERGY;
  energy.spacing = GridSpacing::UNIFORM_LETHARGY;
  energy.min = 1.0e-5;
  energy.max = 1.0e7;
  energy.n_bins = 500;
  energy.outside_policy = GridOutsidePolicy::DROP;

  TallyGridSpec grid;
  grid.dims.push_back(energy);

  std::vector<TallySpec> specs;

  TallySpec flux;
  flux.name = "flux_all";
  flux.quantity = TallyQuantity::FLUX;
  flux.grid = grid;
  specs.push_back(flux);

  const std::vector<RxnType> all_rxn_types = {
      RxnType::ELASTIC, RxnType::DISCRETE_INELASTIC,
      RxnType::CONTINUUM_INELASTIC, RxnType::FISSION,
      RxnType::ABSORPTION, RxnType::N2N, RxnType::N3N};

  for (RxnType rxn : all_rxn_types) {
    TallySpec rr;
    rr.name = "rxn_rate_" + lower_ascii(rxn_type_name(rxn));
    rr.quantity = TallyQuantity::RXN_RATE;
    rr.reactions.types = {rxn};
    rr.grid = grid;
    specs.push_back(rr);
  }

  TallySpec nu_f;
  nu_f.name = "nu_fission_all";
  nu_f.quantity = TallyQuantity::NU_FISSION;
  nu_f.grid = grid;
  specs.push_back(nu_f);

  TallySpec kappa_f;
  kappa_f.name = "kappa_fission_all";
  kappa_f.quantity = TallyQuantity::KAPPA_FISSION;
  kappa_f.grid = grid;
  specs.push_back(kappa_f);

  return specs;
}

inline double sum_weights(const ParticleBankHost &bank) {
  ParticleBankConstView view = bank.view();
  double total = 0.0;
  for (int i = 0; i < bank.size; ++i)
    total += view.w[i];
  return total;
}
