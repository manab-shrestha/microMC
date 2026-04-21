#include "transport.h"
#include "direction.h"
#include "reaction.h"
#include "sampling.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>

namespace {
constexpr double MAX_SOURCE_ENERGY = 1.0e7; // eV
constexpr double MIN_UNIFORM = 1e-16;

inline void copy_particle_between(ParticleBankView dst, int dst_i,
                                  ParticleBankView src, int src_i) {
  dst.x[dst_i] = src.x[src_i];
  dst.y[dst_i] = src.y[src_i];
  dst.z[dst_i] = src.z[src_i];
  dst.E[dst_i] = src.E[src_i];
  dst.Omega_x[dst_i] = src.Omega_x[src_i];
  dst.Omega_y[dst_i] = src.Omega_y[src_i];
  dst.Omega_z[dst_i] = src.Omega_z[src_i];
  dst.w[dst_i] = src.w[src_i];
  dst.macro_xs_t[dst_i] = src.macro_xs_t[src_i];
  dst.alive[dst_i] = src.alive[src_i];
  dst.rxn_nuclide_idx[dst_i] = src.rxn_nuclide_idx[src_i];
  dst.rxn_rxn_idx[dst_i] = src.rxn_rxn_idx[src_i];
  dst.rxn_type[dst_i] = src.rxn_type[src_i];
  dst.rng[dst_i] = src.rng[src_i];
}

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

void calc_xs_soa(TransportStateSoA &state){
  const int n = state.current_bank.size;
  ParticleBankView bank = state.current_bank.view();
  for (int i = 0; i < n ; ++i){
    if (!(bank.alive[i]))
      continue;
    bank.macro_xs_t[i] = total_macro_xs(*state.material, *state.data, bank.E[i]);
    if (!std::isfinite(bank.macro_xs_t[i]) || bank.macro_xs_t[i] <= 0.0)
      bank.alive[i] = 0;
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

void advance_soa(TransportStateSoA &state) {
  const int n = state.current_bank.size;
  ParticleBankView bank = state.current_bank.view();
  for (int i = 0; i < n; ++i) {
    if (!bank.alive[i])
      continue;
    if (!std::isfinite(bank.macro_xs_t[i]) || bank.macro_xs_t[i] <= 0.0) {
      bank.alive[i] = 0;
      continue;
    }

    double u = std::max(uniform(bank.rng[i]), MIN_UNIFORM);
    double distance = -std::log(u) / bank.macro_xs_t[i];
    bank.x[i] += distance * bank.Omega_x[i];
    bank.y[i] += distance * bank.Omega_y[i];
    bank.z[i] += distance * bank.Omega_z[i];
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

void sample_reaction_soa(TransportStateSoA &state) {
  const int n = state.current_bank.size;
  ParticleBankView bank = state.current_bank.view();

  for (int i = 0; i < n; ++i) {
    if (!bank.alive[i])
      continue;

    ReactionChoice choice = sample_reaction_and_macro_xs(
        *state.material, *state.data, bank.E[i], bank.rng[i]);

    if (!choice.valid || !std::isfinite(choice.macro_xs_t) ||
        choice.macro_xs_t <= 0.0) {
      bank.alive[i] = 0;
      continue;
    }

    bank.macro_xs_t[i] = choice.macro_xs_t;
    const ReactionSample &rxn = choice.sample;

    bank.rxn_nuclide_idx[i] = rxn.nuclide_idx;
    bank.rxn_rxn_idx[i] = rxn.rxn_idx;
    bank.rxn_type[i] = static_cast<int>(rxn.type);
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

void process_collision_soa(TransportStateSoA &state) {
  const int n = state.current_bank.size;
  ParticleBankView bank = state.current_bank.view();
  ParticleBankView fission_bank = state.fission_bank.view();
  ParticleBankView secondary_bank = state.secondary_bank.view();

  ParticleBankHost next_bank_host;
  next_bank_host.set_capacity(n + n / 8 + 16);
  next_bank_host.set_size(0);
  ParticleBankView next_bank = next_bank_host.view();
  int next_count = 0;

  auto ensure_next_capacity = [&]() {
    if (next_count < next_bank_host.capacity)
      return;
    int grown = std::max(next_bank_host.capacity * 2, next_count + 1);
    if (grown <= 0)
      grown = next_count + 1;
    next_bank_host.set_capacity(grown);
    next_bank = next_bank_host.view();
  };

  state.fission_count = state.fission_bank.size;
  state.secondary_count = 0;
  state.fission_overflow_count = 0;
  state.secondary_overflow_count = 0;

  for (int i = 0; i < n; ++i) {
    if (!bank.alive[i])
      continue;

    Neutron tally_neutron;
    tally_neutron.E = bank.E[i];
    tally_neutron.w = bank.w[i];
    tally_neutron.macro_xs_t = bank.macro_xs_t[i];
    state.tallies.score_collision(tally_neutron, *state.material, *state.data);

    const int nuclide_idx = bank.rxn_nuclide_idx[i];
    const int rxn_idx = bank.rxn_rxn_idx[i];
    const NuclideDescriptor &nuc = state.data->nuclides[nuclide_idx];
    const ReactionDescriptor &rxn = state.data->reactions[rxn_idx];
    const RxnType type = static_cast<RxnType>(bank.rxn_type[i]);
    const int secondary_begin = state.secondary_count;

    switch (type) {
    case RxnType::ELASTIC:
      elastic_scatter_soa(bank, i, nuc, rxn, *state.data,
                          state.material->temperature);
      break;
    case RxnType::DISCRETE_INELASTIC:
      inelastic_scatter_disc_soa(bank, i, nuc, rxn, *state.data,
                                 state.material->temperature);
      break;
    case RxnType::CONTINUUM_INELASTIC:
      inelastic_scatter_cont_soa(bank, i, nuc, rxn, *state.data,
                                 state.material->temperature);
      break;
    case RxnType::FISSION:
      fission_soa(bank, i, rxn, *state.data, fission_bank, state.fission_count,
                  state.fission_overflow_count, state.k_eff);
      break;
    case RxnType::ABSORPTION:
      bank.alive[i] = 0;
      break;
    case RxnType::N2N:
      multiply_soa(bank, i, nuc, rxn, *state.data, state.material->temperature,
                   secondary_bank, state.secondary_count,
                   state.secondary_overflow_count);
      break;
    case RxnType::N3N:
      multiply_soa(bank, i, nuc, rxn, *state.data, state.material->temperature,
                   secondary_bank, state.secondary_count,
                   state.secondary_overflow_count);
      break;
    }

    if (type == RxnType::N2N || type == RxnType::N3N) {
      for (int s = secondary_begin; s < state.secondary_count; ++s) {
        if (!secondary_bank.alive[s])
          continue;
        ensure_next_capacity();
        copy_particle_between(next_bank, next_count, secondary_bank, s);
        ++next_count;
      }
    }

    if (bank.alive[i]) {
      ensure_next_capacity();
      copy_particle_between(next_bank, next_count, bank, i);
      ++next_count;
    }
  }

  next_bank_host.set_size(next_count);
  state.current_bank = std::move(next_bank_host);
  state.fission_bank.set_size(state.fission_count);
  state.secondary_bank.set_size(state.secondary_count);
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
int comb_bank_soa(ParticleBankHost &bank, int size, int n_target, RNG &cycle_rng) {
  if (n_target <= 0) {
    bank.clear_active();
    return 0;
  }
  ParticleBankView in = bank.view();
  double total_w = 0.0;
  for (int i = 0; i < size; ++i)
    total_w += in.w[i];
  if (total_w <= 0.0 || !std::isfinite(total_w)) {
    bank.clear_active();
    return 0;
  }
  const double step = total_w / static_cast<double>(n_target);
  const double u0 = uniform(cycle_rng) * step;
  ParticleBankHost out;
  out.set_capacity(n_target);
  out.set_size(n_target);
  ParticleBankView outv = out.view();
  double cumulative = 0.0;
  int tooth = 0;
  for (int i = 0; i < size && tooth < n_target; ++i) {
    cumulative += in.w[i];
    while (tooth < n_target && (u0 + tooth * step) < cumulative) {
      copy_particle_between(outv, tooth, in, i);
      outv.w[tooth] = step;
      outv.alive[tooth] = 1;
      outv.rng[tooth] = RNG(cycle_rng());
      ++tooth;
    }
  }
  if (tooth == 0) {
    bank.clear_active();
    return 0;
  }
  while (tooth < n_target) {
    copy_particle_fields(outv, tooth, tooth - 1);
    outv.w[tooth] = step;
    outv.alive[tooth] = 1;
    outv.rng[tooth] = RNG(cycle_rng());
    ++tooth;
  }
  bank = std::move(out);
  bank.set_size(n_target);
  return n_target;
}

void event_compact_bank(ParticleBank &bank) {
  auto it = std::partition(bank.begin(), bank.end(),
                           [](const Neutron &p) { return p.alive; });
  bank.erase(it, bank.end());
}

int compact_soa(ParticleBankView bank, int size) {
  int write = 0;
  for (int read = 0; read < size; ++read) {
    if (!bank.alive[read])
      continue;
    if (write != read)
      copy_particle_fields(bank, write, read);
    ++write;
  }
  return write;
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

void init_source_soa(TransportStateSoA &state, int n_particles, bool fixed_source, const double FIXED_SOURCE_ENERGY){
  if (n_particles < 0){
    throw std::runtime_error("init_source_soa: negative n_particles");
  }

  if (state.current_bank.capacity < n_particles){
    state.current_bank.set_capacity(n_particles);
  }
  state.current_bank.set_size(n_particles);
  ParticleBankView bank = state.current_bank.view();
 
  for (int i = 0; i < n_particles; ++i) {
    init_particle_defaults(bank, i);
    bank.rng[i] = RNG(state.rng());
    bank.E[i] = fixed_source ? FIXED_SOURCE_ENERGY : uniform(state.rng) * MAX_SOURCE_ENERGY;
    sample_isodir(bank.Omega_x[i], bank.Omega_y[i], bank.Omega_z[i], bank.rng[i]);
    bank.alive[i] = 1;
    bank.w[i] = 1.0;
  }
}

void transport_cycle_soa(TransportStateSoA &state) {
  while (state.current_bank.size > 0) {
    if (state.fission_bank.capacity < state.current_bank.size * 2)
      state.fission_bank.set_capacity(state.current_bank.size * 2);
    if (state.secondary_bank.capacity < state.current_bank.size + state.current_bank.size / 8 + 16)
      state.secondary_bank.set_capacity(state.current_bank.size + state.current_bank.size / 8 + 16);

    sample_reaction_soa(state);
    advance_soa(state);
    state.secondary_bank.clear_active();
    process_collision_soa(state);
    state.secondary_bank.clear_active();
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
        multiply(neutron, nuc, rxn, *state.data, state.material->temperature,
                 next_bank, state.rng);
        break;
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
