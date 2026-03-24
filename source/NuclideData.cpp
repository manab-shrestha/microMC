#include "NuclideData.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace {
void append_array(std::ifstream& file, std::vector<double>& flat, std::uint64_t n) {
    std::size_t offset = flat.size();
    flat.resize(offset + n);
    file.read(reinterpret_cast<char*>(flat.data() + offset),
              static_cast<std::streamsize>(n * sizeof(double)));
    if (!file) {
        throw std::runtime_error("Failed while reading array data.");
    }
}
} // namespace

static void read_nuclide(NuclideTable& table, const std::string& filename, int ZAID) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::uint64_t nE;
    file.read(reinterpret_cast<char*>(&nE), sizeof(std::uint64_t));

    table.ZAIDs.push_back(ZAID);
    table.offsets.push_back(table.energy.size());
    table.nE.push_back(nE);

    append_array(file, table.energy,   nE);
    append_array(file, table.total,    nE);
    append_array(file, table.elastic,  nE);
    append_array(file, table.capture,  nE);
    append_array(file, table.fission,  nE);
}

NuclideTable load_nuclides() {
    NuclideTable table;

    read_nuclide(table, "xs/bin/H1.bin",   1001);
    read_nuclide(table, "xs/bin/O16.bin",  8016);
    read_nuclide(table, "xs/bin/U235.bin", 92235);
    read_nuclide(table, "xs/bin/U238.bin", 92238);

    /*
    for (std::size_t i = 0; i < table.ZAIDs.size(); ++i) {
        std::cout << "ZAID: " << table.ZAIDs[i] << '\n'
                  << "Number of grid points N: " << table.nE[i] << '\n';
    }
    */

    return table;
}
