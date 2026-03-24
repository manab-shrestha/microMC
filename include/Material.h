#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "NuclideData.h"

#include <cstdint>
#include <utility>
#include <vector>

struct MaterialComponent {
    uint64_t offset;
    double   num_density;
    uint64_t nE;
};

struct Material {
    std::vector<MaterialComponent> components;
};

struct MicroXS {
    double elastic {0.0};
    double capture {0.0};
    double fission {0.0};
    double total   {0.0};

    MicroXS& operator+=(const MicroXS& rhs) {
        elastic += rhs.elastic;
        capture += rhs.capture;
        fission += rhs.fission;
        total   += rhs.total;
        return *this;
    }

    MicroXS& operator*=(double s) {
        elastic *= s;
        capture *= s;
        fission *= s;
        total   *= s;
        return *this;
    }

    friend MicroXS operator+(MicroXS lhs, const MicroXS& rhs) { return lhs += rhs; }
    friend MicroXS operator*(MicroXS lhs, double s)           { return lhs *= s;   }
    friend MicroXS operator*(double s, MicroXS rhs)           { return rhs *= s;   }
};

struct MacroXS {
    double elastic {0.0};
    double capture {0.0};
    double fission {0.0};
    double total   {0.0};
};

Material material_factory(const std::vector<std::pair<int,double>>& composition,
                       const NuclideTable& table);

MacroXS getMacroXS(const NuclideTable& table, const Material& mat, double energy);

#endif
