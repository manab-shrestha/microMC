#ifndef MATERIAL_H
#define MATERIAL_H

struct NuclearData; // forward declaration

static constexpr int MAX_NUCLIDES_PER_MATERIAL = 16;

struct Material {
  const char *name;
  int zaids[MAX_NUCLIDES_PER_MATERIAL]; // physics identity — source of truth
  double number_densities[MAX_NUCLIDES_PER_MATERIAL]; // atoms/cm³
  int n_nuclides;
  int nuclide_ids[MAX_NUCLIDES_PER_MATERIAL]; // resolved indices into
                                              // NuclearData::nuclides
};

// Resolve zaids → nuclide_ids by searching NuclearData.
// Throws std::runtime_error if any ZAID is not found.
void resolve_material(Material &mat, const NuclearData &data);

// Total macroscopic cross section (cm^-1) at energy E (eV).
double total_macro_xs(const Material &mat, const NuclearData &data, double E);

#endif
