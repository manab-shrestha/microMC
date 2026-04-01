#pragma once

#include <cstdint>

struct Material;
struct NuclearData;

void calculate_k_eigenvalue(const Material &mat, const NuclearData &data,
                            int n_particles, int n_inactive, int n_active,
                            uint64_t seed, bool flux_detector);

void calculate_fixed_source(const Material &mat, const NuclearData &data,
                            int n_particles, const double FIXED_SOURCE_ENERGY,
                            int n_active, uint64_t seed, bool flux_detector);
