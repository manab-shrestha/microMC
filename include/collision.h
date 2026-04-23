#pragma once

#include "nuclear_data.h"
#include "particle_bank.h"

void elastic_scatter(ParticleBankView bank, int i,
                     const NuclideDescriptor &nuc,
                     const ReactionDescriptor &rxn,
                     const NuclearData &data, double temperature);

void inelastic_scatter_disc(ParticleBankView bank, int i,
                            const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, double temperature);
void inelastic_scatter_cont(ParticleBankView bank, int i,
                            const NuclideDescriptor &nuc,
                            const ReactionDescriptor &rxn,
                            const NuclearData &data, double temperature);

void fission(ParticleBankView bank, int i, const ReactionDescriptor &rxn,
             const NuclearData &data, ParticleBankView fission_bank,
             int &fission_count, double k_eff);

/*void multiply_analog(ParticleBankView bank, int i, const NuclideDescriptor &nuc,
              const ReactionDescriptor &rxn, const NuclearData &data,
              double temperature, ParticleBankView secondary_bank,
              int &secondary_count);
*/

void multiply_imp(ParticleBankView bank, int i, const NuclideDescriptor &nuc,
                  const ReactionDescriptor &rxn, const NuclearData &data, double temperature);
