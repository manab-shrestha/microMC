#pragma once

#include "material.h"
#include "neutron.h"
#include "particle_bank.h"
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

struct TransportStateSoA {
  ParticleBankHost current_bank;
  ParticleBankHost fission_bank;
  ParticleBankHost secondary_bank;

  int fission_count{0};
  int secondary_count{0};
  int fission_overflow_count{0};
  int secondary_overflow_count{0};

  const Material *material{nullptr};
  const NuclearData *data{nullptr};

  RNG rng{};
  double k_eff{1.0};
  int cycle{0};

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

void calc_xs_soa(TransportStateSoA &state);
void advance_soa(TransportStateSoA &state);
void sample_reaction_soa(TransportStateSoA &state);
void process_collision_soa(TransportStateSoA &state);

int compact_soa(ParticleBankView bank, int size);
int comb_bank_soa(ParticleBankHost &bank, int size, int n_target, RNG &rng);

void init_source_soa(TransportStateSoA &state, int n_particles,
                     bool fixed_source, const double FIXED_SOURCE_ENERGY);
void transport_cycle_soa(TransportStateSoA &state);
