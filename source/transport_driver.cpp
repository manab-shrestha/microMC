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
constexpr int FISSION_BANK_SAFETY_FACTOR = 8;

void reserve_fission_bank(TransportState &state) {
  const int required = state.fission_bank.size +
                       state.current_bank.size * FISSION_BANK_SAFETY_FACTOR;

  if (state.fission_bank.capacity < required)
    state.fission_bank.set_capacity(required);
}
} // namespace

namespace GEO{
bool inside_julia_xy(double x, double y) {
  constexpr double center_x = 0.0;
  constexpr double center_y = 0.0;
  constexpr double physical_half_width = 5.0;
  constexpr double julia_half_width = 1.5;
  constexpr double cr = -0.5, ci = 0.5;
  constexpr int n = 128;
  if (!std::isfinite(x) || !std::isfinite(y))
    return false;
  double zr = (x - center_x) * julia_half_width / physical_half_width;
  double zi = (y - center_y) * julia_half_width / physical_half_width;
  for (int i = 0; i < n; ++i) {
    const double zr2 = zr * zr - zi * zi + cr;
    zi = 2.0 * zr * zi + ci;
    zr = zr2;
    if (zr * zr + zi * zi > 4.0)
      return false;
  }
  return true;
}
}

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

void kill_out_of_bounds(TransportState &state){
  const int n = state.current_bank.size;
  ParticleBankView bank = state.current_bank.view();
  for(int i = 0; i < n; ++i){
    if (!GEO::inside_julia_xy(bank.x[i],bank.y[i])) bank.alive[i] = 0;
  }
}

void collide(TransportState &state) {
  const int n = state.current_bank.size;
  ParticleBankView bank = state.current_bank.view();
  ParticleBankView fission_bank = state.fission_bank.view();

  state.fission_count = state.fission_bank.size;
  for (int i = 0; i < n; ++i) {
    if (!bank.alive[i])
      continue;

    state.tallies.score_collision(bank.x[i], bank.y[i], bank.z[i], bank.E[i],
                                  bank.w[i], bank.macro_xs_t[i],
                                  *state.material, *state.data);

    const int nuclide_idx = bank.rxn_nuclide_idx[i];
    const int rxn_idx = bank.rxn_rxn_idx[i];
    const NuclideDescriptor &nuc = state.data->nuclides[nuclide_idx];
    const ReactionDescriptor &rxn = state.data->reactions[rxn_idx];
    const RxnType type = static_cast<RxnType>(bank.rxn_type[i]);

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
      multiply(bank, i, nuc, rxn, *state.data, state.material->temperature);
      break;
    case RxnType::N3N:
      multiply(bank, i, nuc, rxn, *state.data, state.material->temperature);
      break;
    }
  }
  state.fission_bank.set_size(state.fission_count);
}

void compact_alive(TransportState &state) {
  ParticleBankView bank = state.current_bank.view();
  const int n = state.current_bank.size;
  int write = 0;

  for (int read = 0; read < n; ++read) {
    if (!bank.alive[read])
      continue;
    if (write != read)
      copy_particle_fields(bank, write, read);
    ++write;
  }

  state.current_bank.set_size(write);
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
    reserve_fission_bank(state);
    sample_reaction(state);
    advance(state);
    kill_out_of_bounds(state);
    collide(state);
    compact_alive(state);
  }
}
