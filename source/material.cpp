#include "Material.h"
#include "NuclearData.h"
#include "physics.h"

#include <stdexcept>
#include <string>

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
      throw std::runtime_error(std::string("Material '") + mat.name +
                               "': ZAID " + std::to_string(zaid) +
                               " not found in nuclear data");
  }

  // Serpent style density/fraction conversion
  // mixing atomic & mass forbidden
  bool has_pos = false, has_neg = false;
  for (int i = 0; i < mat.n_nuclides; ++i) {
    if (mat.number_quantity[i] > 0.0)
      has_pos = true;
    else if (mat.number_quantity[i] < 0.0)
      has_neg = true;
    else
      throw std::runtime_error(std::string("Material '") + mat.name +
                               "': zero fraction for ZAID " +
                               std::to_string(mat.zaids[i]));
  }
  if (has_pos && has_neg)
    throw std::runtime_error(std::string("Material '") + mat.name +
                             "': mixed positive/negative fractions — cannot "
                             "mix atomic and mass units");

  bool mass_fracs = has_neg;

  constexpr double N_A = 0.602214076; // barm-cm directly

  // Look up atomic masses (g/mol) for each nuclide
  double M[MAX_NUCLIDES_PER_MATERIAL];
  for (int i = 0; i < mat.n_nuclides; ++i) {
    auto it = mendeleev::ATOMIC_MASS.find(mat.zaids[i]);
    if (it == mendeleev::ATOMIC_MASS.end())
      throw std::runtime_error(std::string("Material '") + mat.name +
                               "': no atomic mass for ZAID " +
                               std::to_string(mat.zaids[i]));
    M[i] = it->second;
  }

  // density == 0 → "sum" mode: number_quantity are number densities in units of
  // 10^24/CM3
  if (mat.density == 0.0) {
    if (mass_fracs)
      throw std::runtime_error(std::string("Material '") + mat.name +
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
      // Negative fracs → mass fractions; convert to atom fractions via M_i
      double sum_wM = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        sum_wM += (-mat.number_quantity[i]) / M[i];
      for (int i = 0; i < mat.n_nuclides; ++i)
        mat.number_densities[i] =
            rho_a * ((-mat.number_quantity[i]) / M[i]) / sum_wM;
    } else {
      // Positive fracs → atom fractions / stoichiometries
      double sum_f = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        sum_f += mat.number_quantity[i];
      for (int i = 0; i < mat.n_nuclides; ++i)
        mat.number_densities[i] = rho_a * mat.number_quantity[i] / sum_f;
    }
  } else {
    // Negative density = mass density (g/cm³)
    double rho = -mat.density;

    if (mass_fracs) {
      // Negative fracs → mass fractions
      double sum_w = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        sum_w += (-mat.number_quantity[i]);
      for (int i = 0; i < mat.n_nuclides; ++i) {
        double w_i = (-mat.number_quantity[i]) / sum_w;
        mat.number_densities[i] = rho * N_A * w_i / M[i];
      }
    } else {
      // Positive fracs → atom fractions / stoichiometries
      double sum_f = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        sum_f += mat.number_quantity[i];
      // Average molar mass from atom fractions
      double M_avg = 0.0;
      for (int i = 0; i < mat.n_nuclides; ++i)
        M_avg += (mat.number_quantity[i] / sum_f) * M[i];
      double N_total = rho * N_A / M_avg;
      for (int i = 0; i < mat.n_nuclides; ++i)
        mat.number_densities[i] = N_total * mat.number_quantity[i] / sum_f;
    }
  }
}

double total_macro_xs(const Material &mat, const NuclearData &data, double E) {
  double sigma_t = 0.0;
  for (int i = 0; i < mat.n_nuclides; ++i) {
    const auto &nuc = data.nuclides[mat.nuclide_ids[i]];
    for (int r = 0; r < nuc.n_reactions; ++r)
      sigma_t += mat.number_densities[i] *
                 lookup_xs(data, nuc, data.reactions[nuc.rxn_offset + r], E);
  }
  return sigma_t;
}
