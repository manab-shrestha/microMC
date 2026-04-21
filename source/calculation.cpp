#include "calculation.h"
#include "tally.h"
#include "transport.h"
#include <algorithm>
#include <chrono>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace {

std::string lower_ascii(std::string s) {
  for (char &c : s)
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  return s;
}

std::vector<TallySpec> default_tally_specs() {
  GridDimSpec energy;
  energy.dim = GridDim::ENERGY;
  energy.spacing = GridSpacing::UNIFORM_LETHARGY;
  energy.min_eV = 1.0e-5;
  energy.max_eV = 1.0e7;
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

double sum_weights(const ParticleBankHost &bank) {
  ParticleBankConstView view = bank.view();
  double total = 0.0;
  for (int i = 0; i < bank.size; ++i)
    total += view.w[i];
  return total;
}

} // namespace

// ── Helper: pirint cycle summary to stdout ──────────────────────────
static void print_cycle_summary(int cycle, int n_inactive, int n_active,
                                double k_eff, bool active, double k_mean,
                                double rel_unc) {
  using std::cout;
  using std::fixed;
  using std::left;
  using std::right;
  using std::setprecision;
  using std::setw;

  if (active) {
    cout << left << setw(15) << "Active cycle" << right << setw(4)
         << (cycle - n_inactive + 1) << " / " << setw(4) << n_active
         << "   k-eff = " << fixed << setprecision(6) << k_mean;

    if (rel_unc >= 0.0) {
      double abs_unc = k_mean * rel_unc;
      cout << " +/- " << setprecision(6) << rel_unc << "   [" << setprecision(6)
           << (k_mean - abs_unc) << " " << (k_mean + abs_unc) << "]";
    }

    cout << "\n";
  } else {
    cout << left << setw(15) << "Inactive cycle" << right << setw(4)
         << (cycle + 1) << " / " << setw(4) << n_inactive
         << "   k-eff = " << fixed << setprecision(6) << k_eff << "\n";
  }
}

void calculate_k_eigenvalue(const Material &mat, const NuclearData &data,
                            int n_particles, int n_inactive, int n_active,
                            uint64_t seed, bool tally_on,
                            const std::vector<TallySpec> &tally_specs,
                            bool use_soa) {
  if (use_soa) {
    TransportStateSoA state;
    state.material = &mat;
    state.data = &data;
    state.rng = RNG(seed);
    state.k_eff = 1.0;
    state.cycle = 0;
    state.k_eff_history.reserve(n_inactive + n_active);
    if (tally_on)
      state.tallies.configure(mat, data,
                              tally_specs.empty() ? default_tally_specs()
                                                  : tally_specs);

    init_source_soa(state, n_particles, false, 0.0);
    auto start = std::chrono::steady_clock::now();

    double k_sum = 0.0, k_sum_sq = 0.0;
    int n_active_so_far = 0;
    std::vector<double> k_eff_active_history;
    std::vector<double> k_eff_active_rel_err_history;
    k_eff_active_history.reserve(n_active);
    k_eff_active_rel_err_history.reserve(n_active);

    std::cout << std::fixed << std::setprecision(5);

    for (int c = 0; c < n_inactive + n_active; ++c) {
      state.cycle = c;

      double w_source = sum_weights(state.current_bank);
      if (w_source <= 0.0 || !std::isfinite(w_source))
        throw std::runtime_error(
            "Source bank extinction: non-positive source weight");

      state.tallies.begin_cycle((c >= n_inactive) && tally_on);
      transport_cycle_soa(state);
      state.tallies.end_cycle();

      double w_fission = sum_weights(state.fission_bank);
      if (w_fission <= 0.0 || !std::isfinite(w_fission))
        throw std::runtime_error(
            "Fission bank extinction: non-positive fission weight");

      state.k_eff = state.k_eff * w_fission / w_source;
      if (!std::isfinite(state.k_eff))
        throw std::runtime_error("Non-finite k_eff encountered");
      state.k_eff_history.push_back(state.k_eff);

      std::swap(state.current_bank, state.fission_bank);
      state.fission_bank.clear_active();
      state.fission_count = 0;
      state.secondary_bank.clear_active();
      state.secondary_count = 0;
      const int n_resampled =
          comb_bank_soa(state.current_bank, state.current_bank.size, n_particles,
                        state.rng);
      state.current_bank.set_size(n_resampled);

      bool active = (c >= n_inactive);
      if (active) {
        ++n_active_so_far;
        k_sum += state.k_eff;
        k_sum_sq += state.k_eff * state.k_eff;
        k_eff_active_history.push_back(state.k_eff);
      }

      if (active) {
        double mean = k_sum / n_active_so_far;
        double rel_unc = -1.0;
        if (n_active_so_far > 1) {
          double variance =
              (k_sum_sq / n_active_so_far - mean * mean) / n_active_so_far;
          variance = std::max(variance, 0.0);
          rel_unc = std::sqrt(variance) / mean;
        }
        k_eff_active_rel_err_history.push_back(rel_unc);
        print_cycle_summary(c, n_inactive, n_active, state.k_eff, true, mean,
                            rel_unc);
      } else {
        print_cycle_summary(c, n_inactive, n_active, state.k_eff, false, 0.0,
                            -1.0);
      }
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\nk-eigenvalue calculation completed in: "
              << elapsed.count() / 1000.0 << " seconds.\n";

    if (tally_on) {
      double final_mean = 0.0;
      double final_rel_err = 0.0;
      if (n_active_so_far > 0) {
        final_mean = k_sum / n_active_so_far;
        if (n_active_so_far > 1 && final_mean != 0.0) {
          double variance =
              (k_sum_sq / n_active_so_far - final_mean * final_mean) /
              n_active_so_far;
          variance = std::max(variance, 0.0);
          final_rel_err = std::sqrt(variance) / final_mean;
        }
      }

      RunMetadata meta;
      meta.mode = "k_eigenvalue";
      meta.n_particles = n_particles;
      meta.n_inactive = n_inactive;
      meta.n_active = n_active;
      meta.seed = seed;
      meta.material_name = std::string(mat.name);
      meta.temperature_K = mat.temperature;
      meta.k_eff_active_history = std::move(k_eff_active_history);
      meta.k_eff_active_rel_err_history = std::move(k_eff_active_rel_err_history);
      meta.k_eff_final_mean = final_mean;
      meta.k_eff_final_rel_err = final_rel_err;

      TallyOutputOptions out;
      out.file_path = "tallies/k_eigenvalue_tallies.json";
      state.tallies.write_json(meta, mat, data, out);
    }

    return;
  }

  TransportState state;
  state.material = &mat;
  state.data = &data;
  state.rng = RNG(seed);
  state.k_eff = 1.0;
  state.cycle = 0;
  state.k_eff_history.reserve(n_inactive + n_active);
  if (tally_on)
    state.tallies.configure(mat, data,
                           tally_specs.empty() ? default_tally_specs()
                                               : tally_specs);

  init_source(state, n_particles, false, 0.0);
  auto start = std::chrono::steady_clock::now();

  double k_sum = 0.0, k_sum_sq = 0.0;
  int n_active_so_far = 0;
  std::vector<double> k_eff_active_history;
  std::vector<double> k_eff_active_rel_err_history;
  k_eff_active_history.reserve(n_active);
  k_eff_active_rel_err_history.reserve(n_active);

  std::cout << std::fixed << std::setprecision(5);

  for (int c = 0; c < n_inactive + n_active; ++c) {
    state.cycle = c;

    double w_source = 0.0;
    for (const auto &particle : state.current_bank)
      w_source += particle.w;

    if (w_source <= 0.0 || !std::isfinite(w_source))
      throw std::runtime_error(
          "Source bank extinction: non-positive source weight");

    state.tallies.begin_cycle((c >= n_inactive) && tally_on);

    transport_cycle(state);
    state.tallies.end_cycle();

    double w_fission = 0.0;
    for (const auto &particle : state.fission_bank)
      w_fission += particle.w;

    if (w_fission <= 0.0 || !std::isfinite(w_fission))
      throw std::runtime_error(
          "Fission bank extinction: non-positive fission weight");

    state.k_eff = state.k_eff * w_fission / w_source;
    if (!std::isfinite(state.k_eff))
      throw std::runtime_error("Non-finite k_eff encountered");
    state.k_eff_history.push_back(state.k_eff);

    std::swap(state.current_bank, state.fission_bank);
    state.fission_bank.clear();
    comb_bank(state.current_bank, n_particles, state.rng);

    bool active = (c >= n_inactive);
    if (active) {
      ++n_active_so_far;
      k_sum += state.k_eff;
      k_sum_sq += state.k_eff * state.k_eff;
      k_eff_active_history.push_back(state.k_eff);
    }

    if (active) {
      double mean = k_sum / n_active_so_far;
      double rel_unc = -1.0;
      if (n_active_so_far > 1) {
        double variance =
            (k_sum_sq / n_active_so_far - mean * mean) / n_active_so_far;
        variance = std::max(variance, 0.0);
        rel_unc = std::sqrt(variance) / mean;
      }
      k_eff_active_rel_err_history.push_back(rel_unc);
      print_cycle_summary(c, n_inactive, n_active, state.k_eff, true, mean,
                          rel_unc);
    } else {
      print_cycle_summary(c, n_inactive, n_active, state.k_eff, false, 0.0,
                          -1.0);
    }
  }

  auto end = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "\nk-eigenvalue calculation completed in: "
            << elapsed.count() / 1000.0 << " seconds.\n";

  if (tally_on) {
    double final_mean = 0.0;
    double final_rel_err = 0.0;
    if (n_active_so_far > 0) {
      final_mean = k_sum / n_active_so_far;
      if (n_active_so_far > 1 && final_mean != 0.0) {
        double variance =
            (k_sum_sq / n_active_so_far - final_mean * final_mean) /
            n_active_so_far;
        variance = std::max(variance, 0.0);
        final_rel_err = std::sqrt(variance) / final_mean;
      }
    }

    RunMetadata meta;
    meta.mode = "k_eigenvalue";
    meta.n_particles = n_particles;
    meta.n_inactive = n_inactive;
    meta.n_active = n_active;
    meta.seed = seed;
    meta.material_name = std::string(mat.name);
    meta.temperature_K = mat.temperature;
    meta.k_eff_active_history = std::move(k_eff_active_history);
    meta.k_eff_active_rel_err_history = std::move(k_eff_active_rel_err_history);
    meta.k_eff_final_mean = final_mean;
    meta.k_eff_final_rel_err = final_rel_err;

    TallyOutputOptions out;
    out.file_path = "tallies/k_eigenvalue_tallies.json";
    state.tallies.write_json(meta, mat, data, out);
  }
}

void calculate_fixed_source(const Material &mat, const NuclearData &data,
                            int n_particles, const double FIXED_SOURCE_ENERGY,
                            int n_active, uint64_t seed, bool tally_on,
                            const std::vector<TallySpec> &tally_specs,
                            bool use_soa) {
  if (use_soa) {
    TransportStateSoA state;
    state.material = &mat;
    state.data = &data;
    state.rng = RNG(seed);
    state.cycle = 0;
    if (tally_on)
      state.tallies.configure(mat, data,
                              tally_specs.empty() ? default_tally_specs()
                                                  : tally_specs);

    auto start = std::chrono::steady_clock::now();
    std::cout << std::fixed << std::setprecision(5);

    for (int c = 0; c < n_active; ++c) {
      state.cycle = c;

      state.current_bank.clear_active();
      state.fission_bank.clear_active();
      state.secondary_bank.clear_active();
      state.fission_count = 0;
      state.secondary_count = 0;
      init_source_soa(state, n_particles, true, FIXED_SOURCE_ENERGY);

      double w_source = sum_weights(state.current_bank);
      if (w_source <= 0.0 || !std::isfinite(w_source))
        throw std::runtime_error(
            "Source bank extinction: non-positive source weight");

      state.tallies.begin_cycle(tally_on);
      transport_cycle_soa(state);
      state.tallies.end_cycle();

      state.fission_bank.clear_active();
      state.fission_count = 0;
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\nFixed-source calculation completed in: "
              << elapsed.count() / 1000.0 << " seconds.\n";

    if (tally_on) {
      RunMetadata meta;
      meta.mode = "fixed_source";
      meta.n_particles = n_particles;
      meta.n_inactive = 0;
      meta.n_active = n_active;
      meta.seed = seed;
      meta.material_name = std::string(mat.name);
      meta.temperature_K = mat.temperature;

      TallyOutputOptions out;
      out.file_path = "tallies/fixed_source_tallies.json";
      state.tallies.write_json(meta, mat, data, out);
    }

    return;
  }

  TransportState state;
  state.material = &mat;
  state.data = &data;
  state.rng = RNG(seed);
  state.cycle = 0;
  if (tally_on)
    state.tallies.configure(mat, data,
                           tally_specs.empty() ? default_tally_specs()
                                               : tally_specs);

  auto start = std::chrono::steady_clock::now();
  std::cout << std::fixed << std::setprecision(5);

  for (int c = 0; c < n_active; ++c) {
    state.cycle = c;

    state.current_bank.clear();
    state.fission_bank.clear();
    init_source(state, n_particles, true, FIXED_SOURCE_ENERGY);

    double w_source = 0.0;
    for (const auto &particle : state.current_bank)
      w_source += particle.w;

    if (w_source <= 0.0 || !std::isfinite(w_source))
      throw std::runtime_error(
          "Source bank extinction: non-positive source weight");

    state.tallies.begin_cycle(tally_on);

    transport_cycle(state);
    state.tallies.end_cycle();

    state.fission_bank.clear();
  }

  auto end = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "\nFixed-source calculation completed in: "
            << elapsed.count() / 1000.0 << " seconds.\n";

  if (tally_on) {
    RunMetadata meta;
    meta.mode = "fixed_source";
    meta.n_particles = n_particles;
    meta.n_inactive = 0;
    meta.n_active = n_active;
    meta.seed = seed;
    meta.material_name = std::string(mat.name);
    meta.temperature_K = mat.temperature;

    TallyOutputOptions out;
    out.file_path = "tallies/fixed_source_tallies.json";
    state.tallies.write_json(meta, mat, data, out);
  }
}
