#pragma once

#include "material.h"
#include "neutron.h"
#include "nuclear_data.h"
#include "rng.h"
#include "tally.h"
#include <vector>

struct TransportState {
  ParticleBank current_bank;
  ParticleBank fission_bank;
  const Material *material;
  const NuclearData *data;
  RNG rng;
  double k_eff;
  int cycle;
  std::vector<double> k_eff_history;
  TallyManager tallies;
};

void event_calc_xs(TransportState &state);
void event_advance(TransportState &state);
void event_sample_reaction(TransportState &state);
void event_process_collision(TransportState &state);
void event_compact_bank(ParticleBank &bank);
void comb_bank(ParticleBank &bank, int n_target, RNG &rng);
void init_source(TransportState &state, int n_particles, bool fixed_source,
                 const double FIXED_SOURCE_ENERGY);
void transport_cycle(TransportState &state);
