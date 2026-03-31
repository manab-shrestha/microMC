#include "material.h"
#include "atomic_mass.h"
#include "nuclear_data.h"
#include "xs_lookup.h"

#include <stdexcept>
#include <string>

namespace {
constexpr double N_A_BARN_CM = 0.602214076; // Avogadro's number in barn-cm
} // namespace

void resolve_material(Material &mat, const NuclearData &data) {
  for (int i = 0; i < mat.n_nuclides; ++i) {
    int zaid = mat.zaids[i];
    bool found = false;
    for (int j = 0; j < data.n_nuclides; ++j) {
      if (data.nuclides[j].zaid == zaid) {
        mat.nuclide_ids[i] = j;
        found = true;
        break;
      }
    }
    if (!found)
      throw std::runtime_error(std::string("Material '") + mat.name.data() +
                               "': ZAID " + std::to_string(zaid) +
                               " not found in nuclear data");
  }

  // Serpent-style density/fraction conversion (mixing atomic & mass forbidden)
  bool has_pos = false, has_neg = false;
  for (int i = 0; i < mat.n_nuclides; ++i) {
    if (mat.number_quantity[i] > 0.0)
      has_pos = true;
    else if (mat.number_quantity[i] < 0.0)
      has_neg = true;
    else
      throw std::runtime_error(std::string("Material '") + mat.name.data() +
                               "': zero fraction for ZAID " +
                               std::to_string(mat.zaids[i]));
  }
  if (has_pos && has_neg)
    throw std::runtime_error(
        std::string("Material '") + mat.name.data() +
        "': mixed positive/negative fractions — cannot "
        "mix atomic and mass units");

  bool mass_fracs = has_neg;

  // Look up atomic masses (amu) for each nuclide
  double M[MAX_NUCLIDES_PER_MATERIAL];
  for (int i = 0; i < mat.n_nuclides; ++i)
    M[i] = mendeleev::atomic_mass(mat.zaids[i]);

  if (mat.density == 0.0) {
    // "Sum" mode: number_quantity are number densities (10^24/cm3)
    if (mass_fracs)
      throw std::runtime_error(std::string("Material '") + mat.name.data() +
                               "': 'sum' density requires atomic fractions");
    double total = 0.0;
    for (int i = 0; i < mat.n_nuclides; ++i) {
      mat.number_densities[i] = mat.number_quantity[i];
      total += mat.number_quantity[i];
    }
    mat.density = total;
    return;
  }

  if (mat.density > 0.0) {
    // Positive density = total atomic density (atoms/barn-cm)
    double rho_a = mat.density;

    if (mass_fracs) {
      double sum_wM = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        sum_wM += (-mat.number_quantity[i]) / M[i];
      for (int i = 0; i < mat.n_nuclides; ++i)
        mat.number_densities[i] =
            rho_a * ((-mat.number_quantity[i]) / M[i]) / sum_wM;
    } else {
      double sum_f = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        sum_f += mat.number_quantity[i];
      for (int i = 0; i < mat.n_nuclides; ++i)
        mat.number_densities[i] = rho_a * mat.number_quantity[i] / sum_f;
    }
  } else {
    // Negative density = mass density (g/cm3)
    double rho = -mat.density;

    if (mass_fracs) {
      double sum_w = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        sum_w += (-mat.number_quantity[i]);
      for (int i = 0; i < mat.n_nuclides; ++i) {
        double w_i = (-mat.number_quantity[i]) / sum_w;
        mat.number_densities[i] = rho * N_A_BARN_CM * w_i / M[i];
      }
    } else {
      double sum_f = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        sum_f += mat.number_quantity[i];
      double M_avg = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        M_avg += (mat.number_quantity[i] / sum_f) * M[i];
      double N_total = rho * N_A_BARN_CM / M_avg;
      for (int i = 0; i < mat.n_nuclides; ++i)
        mat.number_densities[i] = N_total * mat.number_quantity[i] / sum_f;
    }
  }
}

double total_macro_xs(const Material &mat, const NuclearData &data, double E) {
  double macro_xs_t = 0.0;
  for (int i = 0; i < mat.n_nuclides; ++i) {
    const auto &nuc = data.nuclides[mat.nuclide_ids[i]];
    GridIndex gi = find_grid_index(data, nuc, E);
    for (int r = 0; r < nuc.n_reactions; ++r)
      macro_xs_t +=
          mat.number_densities[i] *
          lookup_micro_xs_at(data, data.reactions[nuc.rxn_offset + r], gi);
  }
  return macro_xs_t;
}
