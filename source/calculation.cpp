#include "calculation.h"
#include "transport.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

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

  if (active && rel_unc >= 0.0) {
    double abs_unc = k_mean * rel_unc;

    cout << left << setw(15) << "Active cycle" << right << setw(4)
         << (cycle - n_inactive + 1) << " / " << setw(4) << n_active
         << "   k-eff = " << fixed << setprecision(6) << k_mean << " +/- "
         << setprecision(5) << rel_unc << "   [" << setprecision(6)
         << (k_mean - abs_unc) << " " << (k_mean + abs_unc) << "]\n";
  } else {
    cout << left << setw(15) << "Inactive cycle" << right << setw(4)
         << (cycle + 1) << " / " << setw(4) << n_inactive
         << "   k-eff = " << fixed << setprecision(6) << k_eff << "\n";
  }
}

void calculate_k_eigenvalue(const Material &mat, const NuclearData &data,
                            int n_particles, int n_inactive, int n_active,
                            uint64_t seed, bool flux_detector) {
  TransportState state;
  state.material = &mat;
  state.data = &data;
  state.rng = RNG(seed);
  state.k_eff = 1.0;
  state.cycle = 0;
  state.k_eff_history.reserve(n_inactive + n_active);
  if (flux_detector) {
    state.tally_file.open("tallies/flux_tally.bin", std::ios::binary);
    if (!state.tally_file)
      throw std::runtime_error("Failed to open tallies/flux_tally.bin");
    state.tally_buffer.reserve(static_cast<size_t>(n_particles) * 4);
  }

  init_source(state, n_particles, false, 0.0);
  auto start = std::chrono::steady_clock::now();

  double k_sum = 0.0, k_sum_sq = 0.0;
  int n_active_so_far = 0;

  std::cout << std::fixed << std::setprecision(5);

  for (int c = 0; c < n_inactive + n_active; ++c) {
    state.cycle = c;

    double w_source = 0.0;
    for (const auto &particle : state.current_bank)
      w_source += particle.w;

    if (w_source <= 0.0 || !std::isfinite(w_source))
      throw std::runtime_error(
          "Source bank extinction: non-positive source weight");

    state.scoring_active = (c >= n_inactive) && flux_detector;
    if (state.scoring_active)
      state.tally_buffer.clear();

    transport_cycle(state);
    if (state.scoring_active)
      flush_tally_buffer(state);

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
    }

    if (active && n_active_so_far > 1) {
      double mean = k_sum / n_active_so_far;
      double variance =
          (k_sum_sq / n_active_so_far - mean * mean) / n_active_so_far;
      print_cycle_summary(c, n_inactive, n_active, state.k_eff, true, mean,
                          std::sqrt(variance) / mean);
    } else {
      print_cycle_summary(c, n_inactive, n_active, state.k_eff, active, 0.0,
                          -1.0);
    }
  }

  auto end = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "\nk-eigenvalue calculation completed in: "
            << elapsed.count() / 1000.0 << " seconds.\n";

  state.tally_file.close();
}

void calculate_fixed_source(const Material &mat, const NuclearData &data,
                            int n_particles, const double FIXED_SOURCE_ENERGY,
                            int n_active, uint64_t seed, bool flux_detector) {
  TransportState state;
  state.material = &mat;
  state.data = &data;
  state.rng = RNG(seed);
  state.cycle = 0;
  if (flux_detector) {
    state.tally_file.open("tallies/fixed_src_flux_tally.bin", std::ios::binary);
    if (!state.tally_file)
      throw std::runtime_error(
          "Failed to open tallies/fixed_src_flux_tally.bin");
    state.tally_buffer.reserve(static_cast<size_t>(n_particles) * 4);
  }

  auto start = std::chrono::steady_clock::now();
  std::cout << std::fixed << std::setprecision(5);
  size_t n_scored_collisions = 0;

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

    state.scoring_active = flux_detector;
    if (state.scoring_active)
      state.tally_buffer.clear();

    transport_cycle(state);
    if (state.scoring_active) {
      n_scored_collisions += state.tally_buffer.size() / 2;
      flush_tally_buffer(state);
    }

    state.fission_bank.clear();
  }

  auto end = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "\nFixed-source calculation completed in: "
            << elapsed.count() / 1000.0 << " seconds.\n";

  if (flux_detector)
    std::cout << "Scored collisions: " << n_scored_collisions << "\n";

  if (flux_detector && n_scored_collisions == 0)
    throw std::runtime_error(
        "Fixed-source run produced zero scored collisions; check material "
        "resolution and source settings");

  state.tally_file.close();
}
