#include "../include/NuclideData.h"
#include "../include/Material.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <limits>

#define BARN2CM2 1.0e-24

int main(){
  NuclideTable nuclides;

  try {
    nuclides = load_nuclides();
  }
  catch (const std::exception& e) {
    std::cerr << "Fatal error " << e.what() << '\n';
    return 1;
  }

  const std::vector<std::pair<int,double>> composition {
    {92235, 8.22e20},
    {92238, 2.23e22},
    {8016 , 5.01e22},
    {1001 , 7.15e22}
  };

  Material homog_mixture_1 {material_factory(composition,nuclides)};
  

  double max_e {std::numeric_limits<double>::max()};
  for (std::size_t i = 0; i < nuclides.ZAIDs.size(); ++i)
    max_e = std::min(max_e, nuclides.energy[nuclides.offsets[i] + nuclides.nE[i] - 1]);

  for (double energy {1e-2}; energy < max_e; energy*= 10){
  std::cout << "------------------------------------------------- \n";
  std::cout << "Incident neutron energy: " << energy << " eV \n";
  std::cout << "------------------------------------------------- \n";

  MacroXS mcx {getMacroXS(nuclides,homog_mixture_1,energy)};
  std::cout << "Sigma_s: " << mcx.elastic << "  P(s) = " << mcx.elastic/mcx.total <<
    "\nSigma_c: " << mcx.capture << "  P(c) = " << mcx.capture/mcx.total <<
    "\nSigma_f: " << mcx.fission << "  P(f) = " << mcx.fission/mcx.total <<
    "\nSigma_t: " << mcx.total << "\n\n";
  }

  /*
  for (const auto& e : nuclides.ZAIDs){
    std::cout << e << '\n';
  }
  */


  
  return 0;
}



