#include "calculation.h"
#include "material.h"
#include "nuclear_data.h"

#include <iostream>
#include <string>

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
  Material mat1 = {"mat1",
                   -10.49,
                   900.0,
                   {1001, 8016, 92235, 92238},
                   {0.4940, 0.3461, 0.005678, 0.1541}};
  Material mat2 = {"mat2", -10.0, 900.0, {92235, 92238}, {0.5, 0.5}};
  Material mat3 = {"mat3",
                   -10.49,
                   900.0,
                   {1001, 8016, 92235, 92238},
                   {0.4940 * 2, 0.3461 * 2, 0.005678, 0.1541}};
  Material mat4 = {"mat4",
                   -10.49,
                   900.0,
                   {1001, 8016, 92235, 92238},
                   {0.04940, 0.03461, 0.005678, 0.1541}};
  Material mat5 = {"water", -10.0, 900.0, {1001, 8016}, {2.0, 1.0}};
  try {
    resolve_material(mat1, data);
    resolve_material(mat2, data);
    resolve_material(mat3, data);
    resolve_material(mat4, data);
    resolve_material(mat5, data);

  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  try {
    // calculate_k_eigenvalue(mat1, data, 10000, 100, 1000, 67, flux_detector);
    calculate_fixed_source(mat5, data, 1000, 1.0e5, 1000, 123, flux_detector);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
