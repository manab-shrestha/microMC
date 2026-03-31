#include "material.h"
#include "nuclear_data.h"
#include "transport.h"

#include <iostream>

int main(int argc, char *argv[]) {
  bool flux_detector = (argc > 1 && std::string(argv[1]) == "true");

  NuclearDataHost host;
  try {
    host = load_nuclear_data("xs/bin/nuclear_data.bin");
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  NuclearData data = host.view();

  // Material identical to that in NE8 CW1 and CW2
  Material kryptonite = {
      "kryptonite",
      -10.49,
      {1001, 8016, 92235, 92238},
      {0.4940, 0.3461, 0.005678, 0.1541},
      4,
  };
  try {
    resolve_material(kryptonite, data);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  try {
    run_k_eigenvalue(kryptonite, data, 10000, 20, 100, 123, flux_detector);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
