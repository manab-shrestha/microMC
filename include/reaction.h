#pragma once

#include "nuclear_data.h"
#include "neutron.h"
#include "particle_bank.h"
#include "rng.h"

void elastic_scatter(Neutron &neutron, const NuclideDescriptor &nuc,
                     const ReactionDescriptor &rxn, const NuclearData &data,
                     double temperature, RNG &rng);

void inelastic_scatter_disc(Neutron &neutron, const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, double temperature,
                            RNG &rng);

void inelastic_scatter_cont(Neutron &neutron, const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, double temperature,
                            RNG &rng);

void fission(Neutron &neutron, const ReactionDescriptor &rxn,
             const NuclearData &data, ParticleBank &fission_bank, double k_eff,
             RNG &rng);

void capture(Neutron &neutron);

void multiply(Neutron &neutron, const NuclideDescriptor &nuc,
               const ReactionDescriptor &rxn, const NuclearData &data,
               double temperature, ParticleBank &secondary_bank, RNG &rng);

void elastic_scatter_soa(ParticleBankView bank, int i,
                         const NuclideDescriptor &nuc,
                         const ReactionDescriptor &rxn,
                         const NuclearData &data, double temperature);

void inelastic_scatter_disc_soa(ParticleBankView bank, int i,
                                const NuclideDescriptor &nuc,
                                const ReactionDescriptor &rxn,
                                const NuclearData &data, double temperature);
void inelastic_scatter_cont_soa(ParticleBankView bank, int i,
                                 const NuclideDescriptor &nuc,
                                 const ReactionDescriptor &rxn,
                                 const NuclearData &data, double temperature);

void fission_soa(ParticleBankView bank, int i, const ReactionDescriptor &rxn,
                 const NuclearData &data, ParticleBankView fission_bank,
                 int &fission_count, int &fission_overflow_count,
                 double k_eff);

void multiply_soa(ParticleBankView bank, int i, const NuclideDescriptor &nuc,
                  const ReactionDescriptor &rxn, const NuclearData &data,
                  double temperature, ParticleBankView secondary_bank,
                  int &secondary_count, int &secondary_overflow_count);
