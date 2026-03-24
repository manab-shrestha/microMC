#ifndef NUCLIDEDATA_HPP
#define NUCLIDEDATA_HPP

#include <cstdint>
#include <string>
#include <vector>

struct NuclideTable {
    std::vector<int>      ZAIDs;
    std::vector<uint64_t> offsets; // start index in flat arrays for nuclide i
    std::vector<uint64_t> nE;      // number of energy points for nuclide i

    std::vector<double> energy;
    std::vector<double> total;
    std::vector<double> elastic;
    std::vector<double> capture;
    std::vector<double> fission;
};

NuclideTable load_nuclides();

#endif
