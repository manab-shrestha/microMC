#pragma once

#include "material.h"
#include "nuclear_data.h"
#include "particle_bank.h"
#include "rng.h"
#include "tally.h"
#include <vector>

struct TransportState {
  ParticleBankHost current_bank;
  ParticleBankHost fission_bank;

  int fission_count{0};

  const Material *material{nullptr};
  const NuclearData *data{nullptr};

  RNG rng{};
  double k_eff{1.0};
  int cycle{0};

  std::vector<double> k_eff_history;
  TallyManager tallies;
};

void advance(TransportState &state);
void sample_reaction(TransportState &state);
void collide(TransportState &state);
void kill_out_of_bounds(TransportState &state);
void compact_alive(TransportState &state);

int comb_bank(ParticleBankHost &bank, int size, int n_target, RNG &rng);

void init_source(TransportState &state, int n_particles, bool fixed_source,
                 const double FIXED_SOURCE_ENERGY);
void transport_cycle(TransportState &state);
