#ifndef REACTION_H
#define REACTION_H

#include "NuclearData.h"
#include "neutron.h"
#include "rng.h"

void elastic_scatter(Neutron &neutron, const NuclideDescriptor &nuc,
                     const NuclearData &data, RNG &rng);

void inelastic_scatter_disc(Neutron &neutron, const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, RNG &rng);

void inelastic_scatter_cont(Neutron &neutron, const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, RNG &rng);

void fission(Neutron &neutron, const NuclideDescriptor &nuc,
             const ReactionDescriptor &rxn, const NuclearData &data,
             ParticleBank &fission_bank, double k_eff, RNG &rng);

void capture(Neutron &neutron);

void multiply(Neutron &neutron, const NuclideDescriptor &nuc,
              const ReactionDescriptor &rxn, const NuclearData &data,
              ParticleBank &current_bank, RNG &rng);

#endif
