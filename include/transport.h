#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "Material.h"
#include "NuclearData.h"
#include "neutron.h"
#include "rng.h"
#include <cstdint>
#include <fstream>
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
  std::ofstream tally_file;
  bool scoring_active{false};
};

void event_advance(TransportState &state);
void event_sample_reaction(TransportState &state);
void event_process_collision(TransportState &state);
void event_compact_bank(ParticleBank &bank);
void score_flux(TransportState &state);

void run_eigenvalue(const Material &mat, const NuclearData &data,
                    int n_particles, int n_inactive, int n_active,
                    uint64_t seed);

#endif
