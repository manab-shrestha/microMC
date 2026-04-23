#include "calculation.h"
#include "material.h"
#include "nuclear_data.h"
#include "tally.h"

#include <iostream>
#include <string>

namespace {
std::vector<TallySpec> build_user_tallies() {
  GridDimSpec energy_grid;
  energy_grid.dim = GridDim::ENERGY;
  energy_grid.spacing = GridSpacing::UNIFORM_LETHARGY;
  energy_grid.min_eV = 1.0e-5;
  energy_grid.max_eV = 20.0e6;
  energy_grid.n_bins = 500;
  energy_grid.outside_policy = GridOutsidePolicy::DROP;

  TallyGridSpec grid;
  grid.dims.push_back(energy_grid);

  std::vector<TallySpec> tallies;

  TallySpec flux;
  flux.name = "flux_all";
  flux.quantity = TallyQuantity::FLUX;
  flux.grid = grid;
  tallies.push_back(flux);

  TallySpec fission_rr;
  fission_rr.name = "fission_rate_u235";
  fission_rr.quantity = TallyQuantity::RXN_RATE;
  fission_rr.reactions.types = {RxnType::FISSION};
  fission_rr.nuclides.zaids = {92235};
  fission_rr.grid = grid;
  tallies.push_back(fission_rr);

  TallySpec abs_rr;
  abs_rr.name = "abs_rate_u238";
  abs_rr.quantity = TallyQuantity::RXN_RATE;
  abs_rr.reactions.types = {RxnType::ABSORPTION};
  abs_rr.nuclides.zaids = {92238};
  abs_rr.grid = grid;
  tallies.push_back(abs_rr);

  TallySpec el_rr;
  el_rr.name = "ela_rate_o16";
  el_rr.quantity = TallyQuantity::RXN_RATE;
  el_rr.reactions.types = {RxnType::ELASTIC};
  el_rr.nuclides.zaids = {8016};
  el_rr.grid = grid;
  tallies.push_back(el_rr);

  TallySpec inel_rr;
  inel_rr.name = "inela_rate_u238";
  inel_rr.quantity = TallyQuantity::RXN_RATE;
  inel_rr.reactions.types = {RxnType::CONTINUUM_INELASTIC, RxnType::DISCRETE_INELASTIC};
  inel_rr.nuclides.zaids = {92238};
  inel_rr.grid = grid;
  tallies.push_back(inel_rr);

  TallySpec nu_f;
  nu_f.name = "nu_fission_all";
  nu_f.quantity = TallyQuantity::NU_FISSION;
  nu_f.grid = grid;
  tallies.push_back(nu_f);

  TallySpec kappa_f;
  kappa_f.name = "kappa_fission_all";
  kappa_f.quantity = TallyQuantity::KAPPA_FISSION;
  kappa_f.grid = grid;
  tallies.push_back(kappa_f);

  return tallies;
}

} // namespace

int main(int argc, char *argv[]) {
  bool tally_on = (argc > 1 && std::string(argv[1]) == "true");

  std::vector<TallySpec> tallies = build_user_tallies();

  Material fuel = {"fuel",
                   -10.49,
                   900.0,
                   {1001, 8016, 92235, 92238},
                   {0.4940, 0.3461, 0.005678, 0.1541}};

  Material absorber = {"absorber",
                       -10.49,
                       900.0,
                       {1001, 8016, 92235, 92238, 5010},
                       {0.4940, 0.3461, 0.005678, 0.1541, 0.000325}};

  Material water = {"water", -10.0, 900.0, {1001, 8016}, {2.0, 1.0}};

  Material *all_mats[] = {&fuel, &absorber, &water};

  const std::string xs_path = "/Users/shrestha/endfb-vii.1-hdf5/neutron";
  //const std::string xs_path = "/home/ms3281/endfb-vii.1-hdf5/neutron";

  NuclearDataHost host;
  try {
    host = load_nuclear_data_hdf5(xs_path, all_mats, 3);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }
  NuclearData data = host.view();

  try {
    for (Material *mat : all_mats)
      resolve_material(*mat, data);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  try {
    calculate_k_eigenvalue(fuel, data, 1e5, 50, 500, 987654321, tally_on,
                           tallies);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
