#include "../include/Material.h"
#include "../include/NuclearData.h"
#include "../include/transport.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <ratio>
#include <stdexcept>
#include <type_traits>

int main(int argc, char *argv[]) {
  bool flux_detector = false;

  if (argc > 1) {
    flux_detector = (std::string(argv[1]) == "true") ? true : false;
  }

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
      -10.49,
      {1001, 8016, 92235, 92238},
      // {0.4664, 0.3268, 0.00536, 0.1455},``
      {0.4940, 0.3461, 0.005678, 0.1541},
      4 //
  };
  try {
    resolve_material(fuel, data);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  auto start = std::chrono::steady_clock::now();

  run_eigenvalue(fuel, data, 1e4, 20, 100, 123, flux_detector);

  auto end = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "k-eigenvalue calculation completed in: "
            << elapsed.count() / 1000 << " seconds\n";

  return 0;
}
