#pragma once

#include <string_view>

struct NuclearData;

static constexpr int MAX_NUCLIDES_PER_MATERIAL = 16;

struct Material {
  std::string_view name;
  double density{0.0};
  int zaids[MAX_NUCLIDES_PER_MATERIAL];
  double number_quantity[MAX_NUCLIDES_PER_MATERIAL];
  int n_nuclides;
  double number_densities[MAX_NUCLIDES_PER_MATERIAL] = {};
  int nuclide_ids[MAX_NUCLIDES_PER_MATERIAL] = {};
};

// Resolve zaids -> nuclide_ids by searching NuclearData.
void resolve_material(Material &mat, const NuclearData &data);

// Total macroscopic cross section (cm^-1) at energy E (eV).
double total_macro_xs(const Material &mat, const NuclearData &data, double E);
