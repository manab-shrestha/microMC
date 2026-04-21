#include "transport_driver.h"
#include "direction.h"
#include "collision.h"
#include "sampling.h"
#include <algorithm>
#include <cmath>
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

void advance(TransportState &state) {
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

void sample_reaction(TransportState &state) {
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

void process_collision(TransportState &state) {
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
  for (int i = 0; i < n; ++i) {
    if (!bank.alive[i])
      continue;

    state.tallies.score_collision(bank.E[i], bank.w[i], bank.macro_xs_t[i],
                                  *state.material, *state.data);

    const int nuclide_idx = bank.rxn_nuclide_idx[i];
    const int rxn_idx = bank.rxn_rxn_idx[i];
    const NuclideDescriptor &nuc = state.data->nuclides[nuclide_idx];
    const ReactionDescriptor &rxn = state.data->reactions[rxn_idx];
    const RxnType type = static_cast<RxnType>(bank.rxn_type[i]);
    const int secondary_begin = state.secondary_count;

    switch (type) {
    case RxnType::ELASTIC:
      elastic_scatter(bank, i, nuc, rxn, *state.data,
                      state.material->temperature);
      break;
    case RxnType::DISCRETE_INELASTIC:
      inelastic_scatter_disc(bank, i, nuc, rxn, *state.data,
                             state.material->temperature);
      break;
    case RxnType::CONTINUUM_INELASTIC:
      inelastic_scatter_cont(bank, i, nuc, rxn, *state.data,
                             state.material->temperature);
      break;
    case RxnType::FISSION:
      fission(bank, i, rxn, *state.data, fission_bank, state.fission_count,
              state.k_eff);
      break;
    case RxnType::ABSORPTION:
      bank.alive[i] = 0;
      break;
    case RxnType::N2N:
      multiply(bank, i, nuc, rxn, *state.data, state.material->temperature,
               secondary_bank, state.secondary_count);
      break;
    case RxnType::N3N:
      multiply(bank, i, nuc, rxn, *state.data, state.material->temperature,
               secondary_bank, state.secondary_count);
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

int comb_bank(ParticleBankHost &bank, int size, int n_target, RNG &cycle_rng) {
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

void init_source(TransportState &state, int n_particles, bool fixed_source,
                 const double fixed_source_energy) {
  if (n_particles < 0) {
    throw std::runtime_error("init_source: negative n_particles");
  }

  if (state.current_bank.capacity < n_particles) {
    state.current_bank.set_capacity(n_particles);
  }
  state.current_bank.set_size(n_particles);
  ParticleBankView bank = state.current_bank.view();

  for (int i = 0; i < n_particles; ++i) {
    init_particle_defaults(bank, i);
    bank.rng[i] = RNG(state.rng());
    bank.E[i] =
        fixed_source ? fixed_source_energy : uniform(state.rng) * MAX_SOURCE_ENERGY;
    sample_isodir(bank.Omega_x[i], bank.Omega_y[i], bank.Omega_z[i],
                  bank.rng[i]);
    bank.alive[i] = 1;
    bank.w[i] = 1.0;
  }
}

void transport_cycle(TransportState &state) {
  while (state.current_bank.size > 0) {
    if (state.fission_bank.capacity < state.current_bank.size * 2)
      state.fission_bank.set_capacity(state.current_bank.size * 2);
    if (state.secondary_bank.capacity <
        state.current_bank.size + state.current_bank.size / 8 + 16)
      state.secondary_bank.set_capacity(state.current_bank.size + state.current_bank.size / 8 + 16);

    sample_reaction(state);
    advance(state);
    state.secondary_bank.clear_active();
    process_collision(state);
    state.secondary_bank.clear_active();
  }
}
