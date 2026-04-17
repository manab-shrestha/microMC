#include "transport.h"
#include "direction.h"
#include "reaction.h"
#include "sampling.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr double MAX_SOURCE_ENERGY = 1.0e7; // eV
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
      elastic_scatter(neutron, nuc, rxn, *state.data,
                      state.material->temperature, state.rng);
      break;
    case RxnType::DISCRETE_INELASTIC:
      inelastic_scatter_disc(neutron, nuc, rxn, *state.data,
                             state.material->temperature, state.rng);
      break;
    case RxnType::CONTINUUM_INELASTIC:
      inelastic_scatter_cont(neutron, nuc, rxn, *state.data,
                             state.material->temperature, state.rng);
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
      multiply(neutron, nuc, rxn, *state.data, state.material->temperature,
               secondaries, state.rng);
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

void event_compact_bank(ParticleBank &bank) {
  auto it = std::partition(bank.begin(), bank.end(),
                           [](const Neutron &p) { return p.alive; });
  bank.erase(it, bank.end());
}

// ── Helper: initialise the source bank ──────────────────────────────

void init_source(TransportState &state, int n_particles, bool fixed_source,
                 const double FIXED_SOURCE_ENERGY) {
  state.current_bank.reserve(n_particles);
  for (int i = 0; i < n_particles; ++i) {
    Neutron particle;
    if (fixed_source)
      particle.E = FIXED_SOURCE_ENERGY;
    else
      particle.E = uniform(state.rng) * MAX_SOURCE_ENERGY;
    sample_isodir(particle.Omega_x, particle.Omega_y, particle.Omega_z,
                  state.rng);
    particle.w = 1.0;
    state.current_bank.push_back(particle);
  }
}

// ── Helper: run one transport cycle until the bank is empty ────────

void transport_cycle(TransportState &state) {
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

      state.tallies.score_collision(neutron, *state.material, *state.data);

      const NuclideDescriptor &nuc =
          state.data->nuclides[neutron.rxn.nuclide_idx];
      const ReactionDescriptor &rxn =
          state.data->reactions[neutron.rxn.rxn_idx];

      switch (neutron.rxn.type) {
      case RxnType::ELASTIC:
        elastic_scatter(neutron, nuc, rxn, *state.data,
                        state.material->temperature, state.rng);
        break;
      case RxnType::DISCRETE_INELASTIC:
        inelastic_scatter_disc(neutron, nuc, rxn, *state.data,
                               state.material->temperature, state.rng);
        break;
      case RxnType::CONTINUUM_INELASTIC:
        inelastic_scatter_cont(neutron, nuc, rxn, *state.data,
                               state.material->temperature, state.rng);
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
        multiply(neutron, nuc, rxn, *state.data, state.material->temperature,
                 next_bank, state.rng);
        break;
      }

      if (neutron.alive)
        next_bank.push_back(neutron);
    }

    state.current_bank = std::move(next_bank);
  }
}
