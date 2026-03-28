#include "../include/Material.h"
#include "../include/NuclearData.h"
#include "../include/physics.h"
#include "rng.h"

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
      "UO2+H2O",
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

  /*
  std::random_device rd;
  RNG rng(rd());

  for (int i = 0; i <= 10; ++i) {
    double rng_E = 5e+6 * uniform(rng);
    ReactionSample s = sample_reaction(fuel, data, rng_E, rng);

    switch (s.type) {
    case RxnType::ELASTIC:
      std::cout << "E (ev): " << rng_E
                << " MT 2: sample scattering cosine, change to lab frame, "
                   "apply kinematic laws\n";
      break;

    case RxnType::DISCRETE_INELASTIC:
      std::cout << "E (ev): " << rng_E
                << "MT 51-90: sample scattering cosine, change to lab frame, "
                   "acquire Q value, apply kinematic laws\n";
      break;

    case RxnType::CONTINUUM_INELASTIC:
      std::cout << "E (ev): " << rng_E << "MT 91: sample Kalbach-Mann\n";
      break;

    case RxnType::FISSION:
      std::cout
          << "E (ev): " << rng_E
          << "MT 18: kill neutron, sample multiplicity and do stochastic "
             "rounding, create new neutrons in fission bank, sample energies "
             "of said neutrons, directions of said neutrons are isotropic\n";
      break;

    case RxnType::ABSORPTION:
      std::cout << "E (ev): " << rng_E << "MT 102-107: kill neutron\n";
      break;

    case RxnType::N2N:
      std::cout << "E (ev): " << rng_E
                << "MT 16: sample Kalbach-Mann, add 2 new neutrons to current "
                   "particle bank\n";
      break;

    case RxnType::N3N:
      std::cout << "E (ev): " << rng_E
                << "MT 17: sample Kalbach-Mann, add 3 new neutrons to current "
                   "particle bank\n";
      break;

    default:
      std::cout << "E (ev): " << rng_E
                << "MT ?: invalid/unconsidered reaction channel\n";
      break;
    }
  }
  */

  return 0;
}
