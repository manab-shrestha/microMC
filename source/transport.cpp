#include "transport.h"
#include "direction.h"
#include "reaction.h"
#include "sampling.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace {
constexpr double MAX_SOURCE_ENERGY = 5.0e6; // eV
constexpr double MIN_UNIFORM = 1e-16;
} // namespace

void event_calc_xs(TransportState &state) {
  for (auto &particle : state.current_bank) {
    if (!particle.alive)
      continue;
    particle.macro_xs_t =
        total_macro_xs(*state.material, *state.data, particle.E);
    if (!std::isfinite(particle.macro_xs_t) || particle.macro_xs_t <= 0.0)
      particle.alive = false;
  }
}

void event_advance(TransportState &state) {
  for (auto &particle : state.current_bank) {
    if (!particle.alive)
      continue;
    if (!std::isfinite(particle.macro_xs_t) || particle.macro_xs_t <= 0.0) {
      particle.alive = false;
      continue;
    }
    double u = std::max(uniform(state.rng), MIN_UNIFORM);
    double distance = -std::log(u) / particle.macro_xs_t;
    particle.x += distance * particle.Omega_x;
    particle.y += distance * particle.Omega_y;
    particle.z += distance * particle.Omega_z;
  }
}

void event_sample_reaction(TransportState &state) {
  for (auto &particle : state.current_bank) {
    if (!particle.alive)
      continue;
    if (particle.macro_xs_t <= 0.0) {
      particle.alive = false;
      continue;
    }
    particle.rxn = sample_reaction(*state.material, *state.data, particle.E,
                                   particle.macro_xs_t, state.rng);
  }
}

void event_process_collision(TransportState &state) {
  int bank_size = static_cast<int>(state.current_bank.size());
  ParticleBank secondaries;
  secondaries.reserve(bank_size / 8 + 16);
  for (int i = 0; i < bank_size; ++i) {
    Neutron &neutron = state.current_bank[i];
    if (!neutron.alive)
      continue;

    const NuclideDescriptor &nuc =
        state.data->nuclides[neutron.rxn.nuclide_idx];
    const ReactionDescriptor &rxn = state.data->reactions[neutron.rxn.rxn_idx];

    switch (neutron.rxn.type) {
    case RxnType::ELASTIC:
      elastic_scatter(neutron, nuc, rxn, *state.data, state.rng);
      break;
    case RxnType::DISCRETE_INELASTIC:
      inelastic_scatter_disc(neutron, nuc, rxn, *state.data, state.rng);
      break;
    case RxnType::CONTINUUM_INELASTIC:
      inelastic_scatter_cont(neutron, rxn, *state.data, state.rng);
      break;
    case RxnType::FISSION:
      fission(neutron, rxn, *state.data, state.fission_bank, state.k_eff,
              state.rng);
      break;
    case RxnType::ABSORPTION:
      capture(neutron);
      break;
    case RxnType::N2N:
    case RxnType::N3N:
      multiply(neutron, rxn, *state.data, secondaries, state.rng);
      break;
    }
  }
  state.current_bank.insert(state.current_bank.end(), secondaries.begin(),
                            secondaries.end());
}

void comb_bank(ParticleBank &bank, int n_target, RNG &rng) {
  if (n_target <= 0) {
    bank.clear();
    return;
  }

  double total_w = 0.0;
  for (const auto &particle : bank)
    total_w += particle.w;

  if (total_w <= 0.0 || !std::isfinite(total_w)) {
    bank.clear();
    return;
  }

  double step = total_w / n_target;
  double u = uniform(rng) * step;

  ParticleBank combed;
  combed.reserve(n_target);

  double cumulative = 0.0;
  int tooth = 0;
  for (const auto &particle : bank) {
    cumulative += particle.w;
    while (tooth < n_target && u + tooth * step < cumulative) {
      Neutron selected = particle;
      selected.w = step;
      combed.push_back(selected);
      ++tooth;
    }
  }

  if (combed.empty()) {
    bank.clear();
    return;
  }

  while (static_cast<int>(combed.size()) < n_target)
    combed.push_back(combed.back());

  bank = std::move(combed);
}

void score_flux(TransportState &state) {
  for (const auto &neutron : state.current_bank) {
    if (!neutron.alive)
      continue;
    if (!std::isfinite(neutron.macro_xs_t) || neutron.macro_xs_t <= 0.0)
      continue;
    double score = neutron.w / neutron.macro_xs_t;
    state.tally_buffer.push_back(neutron.E);
    state.tally_buffer.push_back(score);
  }
}

static void flush_tally_buffer(TransportState &state) {
  if (!state.tally_file.is_open() || state.tally_buffer.empty())
    return;
  state.tally_file.write(
      reinterpret_cast<const char *>(state.tally_buffer.data()),
      static_cast<std::streamsize>(state.tally_buffer.size() * sizeof(double)));
  state.tally_buffer.clear();
}

void event_compact_bank(ParticleBank &bank) {
  auto it = std::partition(bank.begin(), bank.end(),
                           [](const Neutron &p) { return p.alive; });
  bank.erase(it, bank.end());
}

// ── Helper: initialise the source bank with uniform energy ─────────

static void init_source(TransportState &state, int n_particles) {
  state.current_bank.reserve(n_particles);
  for (int i = 0; i < n_particles; ++i) {
    Neutron particle;
    particle.E = uniform(state.rng) * MAX_SOURCE_ENERGY;
    sample_isodir(particle.Omega_x, particle.Omega_y, particle.Omega_z,
                  state.rng);
    particle.w = 1.0;
    state.current_bank.push_back(particle);
  }
}

// ── Helper: run one transport cycle until the bank is empty ────────

static void transport_cycle(TransportState &state) {
  while (!state.current_bank.empty()) {
    if (state.fission_bank.capacity() < state.current_bank.size() * 2)
      state.fission_bank.reserve(state.current_bank.size() * 2);

    int bank_size = static_cast<int>(state.current_bank.size());
    ParticleBank next_bank;
    next_bank.reserve(static_cast<size_t>(bank_size) + bank_size / 8 + 16);

    for (int i = 0; i < bank_size; ++i) {
      Neutron &neutron = state.current_bank[i];
      if (!neutron.alive)
        continue;

      ReactionChoice choice = sample_reaction_and_macro_xs(
          *state.material, *state.data, neutron.E, state.rng);
      if (!choice.valid || !std::isfinite(choice.macro_xs_t) ||
          choice.macro_xs_t <= 0.0) {
        neutron.alive = false;
        continue;
      }

      neutron.macro_xs_t = choice.macro_xs_t;
      neutron.rxn = choice.sample;

      double u = std::max(uniform(state.rng), MIN_UNIFORM);
      double distance = -std::log(u) / neutron.macro_xs_t;
      neutron.x += distance * neutron.Omega_x;
      neutron.y += distance * neutron.Omega_y;
      neutron.z += distance * neutron.Omega_z;

      if (state.scoring_active) {
        double score = neutron.w / neutron.macro_xs_t;
        state.tally_buffer.push_back(neutron.E);
        state.tally_buffer.push_back(score);
      }

      const NuclideDescriptor &nuc =
          state.data->nuclides[neutron.rxn.nuclide_idx];
      const ReactionDescriptor &rxn =
          state.data->reactions[neutron.rxn.rxn_idx];

      switch (neutron.rxn.type) {
      case RxnType::ELASTIC:
        elastic_scatter(neutron, nuc, rxn, *state.data, state.rng);
        break;
      case RxnType::DISCRETE_INELASTIC:
        inelastic_scatter_disc(neutron, nuc, rxn, *state.data, state.rng);
        break;
      case RxnType::CONTINUUM_INELASTIC:
        inelastic_scatter_cont(neutron, rxn, *state.data, state.rng);
        break;
      case RxnType::FISSION:
        fission(neutron, rxn, *state.data, state.fission_bank, state.k_eff,
                state.rng);
        break;
      case RxnType::ABSORPTION:
        capture(neutron);
        break;
      case RxnType::N2N:
      case RxnType::N3N:
        multiply(neutron, rxn, *state.data, next_bank, state.rng);
        break;
      }

      if (neutron.alive)
        next_bank.push_back(neutron);
    }

    state.current_bank = std::move(next_bank);
  }
}

// ── Helper: print cycle summary to stdout ──────────────────────────

static void print_cycle_summary(int cycle, int n_inactive, int n_active,
                                double k_eff, bool active, double k_mean,
                                double rel_unc) {
  using std::cout;
  using std::fixed;
  using std::setprecision;
  using std::setw;

  if (active && rel_unc >= 0.0) {
    double abs_unc = k_mean * rel_unc;

    cout << "Active   " << setw(4) << (cycle - n_inactive + 1) << "/" << setw(4)
         << n_active << "   k_eff = " << fixed << setprecision(6) << setw(10)
         << k_mean << "   rel err = " << setprecision(5) << rel_unc << "   ["
         << setprecision(6) << (k_mean - abs_unc) << ", " << (k_mean + abs_unc)
         << "]\n";
  } else {
    cout << "Inactive " << setw(4) << (cycle + 1) << "/" << setw(4)
         << n_inactive << "   k_eff = " << fixed << setprecision(6) << setw(10)
         << k_eff << "\n";
  }
}
// ── Main eigenvalue driver ─────────────────────────────────────────

void run_k_eigenvalue(const Material &mat, const NuclearData &data,
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

  init_source(state, n_particles);
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
