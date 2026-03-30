#include "../include/transport.h"
#include "../include/physics.h"
#include "../include/reaction.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

void event_advance(TransportState &state) {
  for (auto &n : state.current_bank) {
    double sigma_t = total_macro_xs(*state.material, *state.data, n.E);
    double d = -std::log(uniform(state.rng)) / sigma_t;
    n.x += d * n.Omega_x;
    n.y += d * n.Omega_y;
    n.z += d * n.Omega_z;
  }
}

void event_sample_reaction(TransportState &state) {
  for (int i{0}; i < static_cast<int>(state.current_bank.size()); ++i) {
    state.current_bank[i].rxn = sample_reaction(
        *state.material, *state.data, state.current_bank[i].E, state.rng);
  }
}

void event_process_collision(TransportState &state) {
  int n = static_cast<int>(state.current_bank.size());
  for (int i = 0; i < n; ++i) {
    Neutron &neutron = state.current_bank[i];
    if (!neutron.alive)
      continue;

    const NuclideDescriptor &nuc = state.data->nuclides[neutron.rxn.nuclide_id];
    const ReactionDescriptor &rxn = state.data->reactions[neutron.rxn.rxn_id];

    switch (static_cast<RxnType>(neutron.rxn.type)) {
    case RxnType::ELASTIC:
      elastic_scatter(neutron, nuc, *state.data, state.rng);
      break;
    case RxnType::DISCRETE_INELASTIC:
      inelastic_scatter_disc(neutron, nuc, rxn, *state.data, state.rng);
      break;
    case RxnType::CONTINUUM_INELASTIC:
      inelastic_scatter_cont(neutron, nuc, rxn, *state.data, state.rng);
      break;
    case RxnType::FISSION:
      fission(neutron, nuc, rxn, *state.data, state.fission_bank, state.k_eff,
              state.rng);
      break;
    case RxnType::ABSORPTION:
      capture(neutron);
      break;
    case RxnType::N2N:
    case RxnType::N3N:
      multiply(neutron, nuc, rxn, *state.data, state.current_bank, state.rng);
      break;
    }
  }
}

void comb_bank(ParticleBank &bank, int n_target, RNG &rng) {
  double total_w = 0.0;
  for (const auto &n : bank)
    total_w += n.w;

  double step = total_w / n_target;
  double u = uniform(rng) * step;

  ParticleBank combed;
  combed.reserve(n_target);

  double cumulative = 0.0;
  int tooth = 0;
  for (const auto &n : bank) {
    cumulative += n.w;
    while (u + tooth * step < cumulative) {
      Neutron selected = n;
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
    double sigma_t = total_macro_xs(*state.material, *state.data, neutron.E);
    double score = neutron.w / sigma_t;
    state.tally_file.write(reinterpret_cast<const char *>(&neutron.E),
                           sizeof(double));
    state.tally_file.write(reinterpret_cast<const char *>(&score),
                           sizeof(double));
  }
}

void event_compact_bank(ParticleBank &bank) {
  auto it = std::partition(bank.begin(), bank.end(),
                           [](const Neutron &n) { return n.alive; });
  bank.erase(it, bank.end());
}

void run_eigenvalue(const Material &mat, const NuclearData &data,
                    int n_particles, int n_inactive, int n_active,
                    uint64_t seed) {
  TransportState state;
  state.material = &mat;
  state.data = &data;
  state.rng = RNG(seed);
  state.k_eff = 1.0;
  state.cycle = 0;
  state.k_eff_history.reserve(n_inactive + n_active);
  state.tally_file.open("tallies/flux_tally.bin", std::ios::binary);

  for (int i = 0; i < n_particles; ++i) {
    Neutron n;
    n.E = uniform(state.rng) * 5e6;
    sample_isodir(n.Omega_x, n.Omega_y, n.Omega_z, state.rng);
    n.w = 1.0;
    state.current_bank.push_back(n);
  }

  double k_sum = 0.0, k_sum_sq = 0.0;
  int n_active_so_far = 0;

  for (int c = 0; c < n_inactive + n_active; ++c) {
    state.cycle = c;

    double W_source = 0.0;
    for (const auto &n : state.current_bank)
      W_source += n.w;

    state.scoring_active = (c >= n_inactive);

    while (!state.current_bank.empty()) {
      event_advance(state);
      event_sample_reaction(state);
      if (state.scoring_active)
        score_flux(state);
      event_process_collision(state);
      event_compact_bank(state.current_bank);
    }

    double W_fission = 0.0;
    for (const auto &n : state.fission_bank)
      W_fission += n.w;
    state.k_eff = state.k_eff * W_fission / W_source;
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

    std::cout << std::fixed << std::setprecision(5);
    if (active && n_active_so_far > 1) {
      double mean = k_sum / n_active_so_far;
      double variance =
          (k_sum_sq / n_active_so_far - mean * mean) / n_active_so_far;
      double rel_unc = std::sqrt(variance) / mean;
      std::cout << "cycle " << std::setw(4) << c << "  k_eff = " << state.k_eff
                << "  mean = " << mean << "  +/- " << rel_unc << "\n";
    } else {
      std::cout << "cycle " << std::setw(4) << c << "  k_eff = " << state.k_eff
                << (active ? "  (active)" : "  (inactive)") << "\n";
    }
  }

  state.tally_file.close();

  double mean = k_sum / n_active;
  double variance = (k_sum_sq / n_active - mean * mean) / n_active;
  std::cout << "\nfinal k_eff = " << mean << "  +/- "
            << std::sqrt(variance) / mean << '\n';
}
