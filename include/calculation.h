#pragma once

#include "tally.h"

#include <cstdint>
#include <vector>

struct Material;
struct NuclearData;

void calculate_k_eigenvalue(const Material &mat, const NuclearData &data,
                            int n_particles, int n_inactive, int n_active,
                            uint64_t seed, bool tally_on,
                            const std::vector<TallySpec> &tally_specs = {},
                            bool use_soa = false);

void calculate_fixed_source(const Material &mat, const NuclearData &data,
                            int n_particles, const double FIXED_SOURCE_ENERGY,
                            int n_active, uint64_t seed, bool tally_on,
                            const std::vector<TallySpec> &tally_specs = {},
                            bool use_soa = false);
