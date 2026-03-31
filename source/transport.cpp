#include "transport.h"
#include "direction.h"
#include "reaction.h"
#include "sampling.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

namespace {
constexpr double MAX_SOURCE_ENERGY = 5.0e6; // eV
} // namespace

void event_calc_xs(TransportState &state) {
  for (auto &particle : state.current_bank) {
    if (!particle.alive)
      continue;
    particle.macro_xs_t =
        total_macro_xs(*state.material, *state.data, particle.E);
  }
}

void event_advance(TransportState &state) {
  for (auto &particle : state.current_bank) {
    double distance = -std::log(uniform(state.rng)) / particle.macro_xs_t;
    particle.x += distance * particle.Omega_x;
    particle.y += distance * particle.Omega_y;
    particle.z += distance * particle.Omega_z;
  }
}

void event_sample_reaction(TransportState &state) {
  for (auto &particle : state.current_bank) {
    particle.rxn = sample_reaction(*state.material, *state.data, particle.E,
                                   particle.macro_xs_t, state.rng);
  }
}

void event_process_collision(TransportState &state) {
  int bank_size = static_cast<int>(state.current_bank.size());
  ParticleBank secondaries;
  for (int i = 0; i < bank_size; ++i) {
    Neutron &neutron = state.current_bank[i];
    if (!neutron.alive)
      continue;

    const NuclideDescriptor &nuc =
        state.data->nuclides[neutron.rxn.nuclide_idx];
    const ReactionDescriptor &rxn =
        state.data->reactions[neutron.rxn.rxn_idx];

    switch (neutron.rxn.type) {
    case RxnType::ELASTIC:
      elastic_scatter(neutron, nuc, *state.data, state.rng);
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
  double total_w = 0.0;
  for (const auto &particle : bank)
    total_w += particle.w;

  double step = total_w / n_target;
  double u = uniform(rng) * step;

  ParticleBank combed;
  combed.reserve(n_target);

  double cumulative = 0.0;
  int tooth = 0;
  for (const auto &particle : bank) {
    cumulative += particle.w;
    while (u + tooth * step < cumulative) {
      Neutron selected = particle;
      selected.w = step;
      combed.push_back(selected);
      ++tooth;
    }
  }

  bank = std::move(combed);
}

void score_flux(TransportState &state) {
  for (const auto &neutron : state.current_bank) {
    if (!neutron.alive)
      continue;
    double score = neutron.w / neutron.macro_xs_t;
    state.tally_file.write(reinterpret_cast<const char *>(&neutron.E),
                           sizeof(double));
    state.tally_file.write(reinterpret_cast<const char *>(&score),
                           sizeof(double));
  }
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
    event_calc_xs(state);
    event_advance(state);
    event_sample_reaction(state);
    if (state.scoring_active)
      score_flux(state);
    event_process_collision(state);
    event_compact_bank(state.current_bank);
  }
}

// ── Helper: print cycle summary to stdout ──────────────────────────

static void print_cycle_summary(int cycle, double k_eff, bool active,
                                double k_mean, double rel_unc) {
  if (active && rel_unc >= 0.0) {
    std::cout << "cycle " << std::setw(4) << cycle << "  k_eff = " << k_eff
              << "  mean = " << k_mean << "  +/- " << rel_unc << "\n";
  } else {
    std::cout << "cycle " << std::setw(4) << cycle << "  k_eff = " << k_eff
              << (active ? "  (active)" : "  (inactive)") << "\n";
  }
}

// ── Main eigenvalue driver ─────────────────────────────────────────

void run_eigenvalue(const Material &mat, const NuclearData &data,
                    int n_particles, int n_inactive, int n_active,
                    uint64_t seed, bool flux_detector) {
  TransportState state;
  state.material = &mat;
  state.data = &data;
  state.rng = RNG(seed);
  state.k_eff = 1.0;
  state.cycle = 0;
  state.k_eff_history.reserve(n_inactive + n_active);
  state.tally_file.open("tallies/flux_tally.bin", std::ios::binary);

  init_source(state, n_particles);

  double k_sum = 0.0, k_sum_sq = 0.0;
  int n_active_so_far = 0;

  std::cout << std::fixed << std::setprecision(5);

  for (int c = 0; c < n_inactive + n_active; ++c) {
    state.cycle = c;

    double w_source = 0.0;
    for (const auto &particle : state.current_bank)
      w_source += particle.w;

    state.scoring_active = (c >= n_inactive) && flux_detector;

    transport_cycle(state);

    double w_fission = 0.0;
    for (const auto &particle : state.fission_bank)
      w_fission += particle.w;
    state.k_eff = state.k_eff * w_fission / w_source;
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
      print_cycle_summary(c, state.k_eff, true, mean,
                          std::sqrt(variance) / mean);
    } else {
      print_cycle_summary(c, state.k_eff, active, 0.0, -1.0);
    }
  }

  state.tally_file.close();

  double mean = k_sum / n_active;
  double variance = (k_sum_sq / n_active - mean * mean) / n_active;
  std::cout << "\nfinal k_eff = " << mean << "  +/- "
            << std::sqrt(variance) / mean << '\n';
}
