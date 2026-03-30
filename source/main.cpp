#include "../include/Material.h"
#include "../include/NuclearData.h"
#include "../include/transport.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

int main() {
  NuclearDataHost host;
  try {
    host = load_nuclear_data("xs/bin/nuclear_data.bin");
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  NuclearData data = host.view();

  // Material identiacal to that in NE8 CW1 and CW2
  Material fuel = {
      "blob",
      {1001, 8016, 92235, 92238},           // ZAIDs: H1, O16, U235, U238
      {7.15e22, 5.01e22, 8.22e20, 2.23e22}, // Number density /cm3
      4,
      {} // nuclide_ids filled by resolve_material
  };
  try {
    resolve_material(fuel, data);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  run_eigenvalue(fuel, data, 1000, 50, 1000, 123);

  return 0;
}
