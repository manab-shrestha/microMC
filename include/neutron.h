#ifndef NEUTRON_H
#define NEUTRON_H

#include "physics.h"
#include <NuclearData.h>
#include <vector>

struct Neutron {
  double x{0.0};
  double y{0.0};
  double z{0.0};
  double E{0.0};
  double Omega_x{0.0};
  double Omega_y{0.0};
  double Omega_z{0.0};
  double w{1.0};
  bool alive{true};
  ReactionSample rxn;
};

using ParticleBank = std::vector<Neutron>;

#endif
