#ifndef MATERIAL_H
#define MATERIAL_H

#include "atomic_mass.h"

struct NuclearData; // forward declaration

static constexpr int MAX_NUCLIDES_PER_MATERIAL = 16;

struct Material {
  const char *name;
  double density{0.0};                  // material density
  int zaids[MAX_NUCLIDES_PER_MATERIAL]; // physics identity — source of truth
  double number_quantity[MAX_NUCLIDES_PER_MATERIAL];
  int n_nuclides;
  double number_densities[MAX_NUCLIDES_PER_MATERIAL] = {}; // atoms/cm³
  int nuclide_ids[MAX_NUCLIDES_PER_MATERIAL] = {}; // resolved indices into
                                                   // NuclearData::nuclides
};

// Resolve zaids → nuclide_ids by searching NuclearData.
// Throws std::runtime_error if any ZAID is not found.
void resolve_material(Material &mat, const NuclearData &data);

// Total macroscopic cross section (cm^-1) at energy E (eV).
double total_macro_xs(const Material &mat, const NuclearData &data, double E);

#endif
