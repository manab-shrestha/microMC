#include "Material.h"

#include <algorithm>
#include <stdexcept>
#include <string>

Material material_factory(const std::vector<std::pair<int,double>>& composition,
                       const NuclideTable& table) {
    Material mat;
    for (const auto& comp : composition) {
        int    ZAID         {comp.first};
        double num_density {comp.second};

        int index = -1;
        for (std::size_t i = 0; i < table.ZAIDs.size(); ++i) {
            if (table.ZAIDs[i] == ZAID) {
                index = static_cast<int>(i);
                break;
            }
        }
        if (index == -1) {
            throw std::runtime_error("ZAID " + std::to_string(ZAID) +
                                     " not found in NuclideTable.");
        }
        mat.components.push_back({table.offsets[index], num_density, table.nE[index]});
    }
    return mat;
}

static MicroXS getMicroXS(uint64_t offset, uint64_t nE, double energy, const NuclideTable& table) {
    auto start {table.energy.begin() + offset};
    auto end   {start + nE};

    auto it {std::lower_bound(start, end, energy)};

    if (it == end)   --it;
    if (it == start) ++it;

    uint64_t hi {it - table.energy.begin()};
    uint64_t lo {hi - 1};

    double t = (energy - table.energy[lo]) / (table.energy[hi] - table.energy[lo]);

    return {
        table.elastic[lo] + t * (table.elastic[hi] - table.elastic[lo]),
        table.capture[lo] + t * (table.capture[hi] - table.capture[lo]),
        table.fission[lo] + t * (table.fission[hi] - table.fission[lo]),
        table.total[lo]   + t * (table.total[hi]   - table.total[lo])
    };
}

MacroXS getMacroXS(const NuclideTable& table, const Material& mat, double energy) {
    MacroXS macroxs;

    for (const auto& comp : mat.components) {
        MicroXS micro = getMicroXS(comp.offset, comp.nE, energy, table);
        macroxs.elastic += comp.num_density * micro.elastic;
        macroxs.capture += comp.num_density * micro.capture;
        macroxs.fission += comp.num_density * micro.fission;
        macroxs.total   += comp.num_density * micro.total;
    }

    return macroxs;
}
