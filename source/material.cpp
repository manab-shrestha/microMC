#include "Material.h"
#include "NuclearData.h"
#include "physics.h"

#include <stdexcept>
#include <string>

void resolve_material(Material& mat, const NuclearData& data) {
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
            throw std::runtime_error(
                std::string("Material '") + mat.name + "': ZAID " +
                std::to_string(zaid) + " not found in nuclear data");
    }
}

double total_macro_xs(const Material& mat, const NuclearData& data, double E) {
    double sigma_t = 0.0;
    for (int i = 0; i < mat.n_nuclides; ++i) {
        const auto& nuc = data.nuclides[mat.nuclide_ids[i]];
        for (int r = 0; r < nuc.n_reactions; ++r)
            sigma_t += mat.number_densities[i] * lookup_xs(data, nuc, data.reactions[nuc.rxn_offset + r], E);
    }
    return sigma_t;
}
