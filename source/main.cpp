#include "calculation.h"
#include "material.h"
#include "nuclear_data.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  bool tally_on = (argc > 1 && std::string(argv[1]) == "true");

  Material fuel = {"fuel",
                   -10.49,
                   900.0,
                   {1001, 8016, 92235, 92238},
                   {0.4940, 0.3461, 0.005678, 0.1541}};

  Material water = {"water", -10.0, 900.0, {1001, 8016}, {2.0, 1.0}};

  Material *all_mats[] = {&fuel,&water};

  const std::string xs_path = "/Users/shrestha/endfb-vii.1-hdf5/neutron";
  //const std::string xs_path = "/home/ms3281/endfb-vii.1-hdf5/neutron"


  NuclearDataHost host;
  try {
    host = load_nuclear_data_hdf5(xs_path, all_mats, 2);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  NuclearData data = host.view();

  try {
    for (Material *mat : all_mats)
      resolve_material(*mat, data);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  try {
    calculate_k_eigenvalue(fuel, data, 50000, 100, 1000, 1, tally_on);
    calculate_fixed_source(water,data,5000,5.0e5,1000,1,tally_on);
  } catch (const std::exception &e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
