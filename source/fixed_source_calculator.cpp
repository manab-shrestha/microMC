#include "calculation.h"
#include "calcualtion_internal.h"
#include "transport_driver.h"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

void calculate_fixed_source(const Material &mat, const NuclearData &data,
                            int n_particles, const double fixed_source_energy,
                            int n_active, uint64_t seed, bool tally_on,
                            const std::vector<TallySpec> &tally_specs) {
  TransportState state;
  state.material = &mat;
  state.data = &data;
  state.rng = RNG(seed);
  state.cycle = 0;
  if (tally_on)
    state.tallies.configure(mat, data,
                            tally_specs.empty() ? default_tally_specs()
                                                : tally_specs);

  auto start = std::chrono::steady_clock::now();
  std::cout << std::fixed << std::setprecision(5);

  for (int c = 0; c < n_active; ++c) {
    state.cycle = c;

    state.current_bank.clear_active();
    state.fission_bank.clear_active();
    state.secondary_bank.clear_active();
    state.fission_count = 0;
    state.secondary_count = 0;
    init_source(state, n_particles, true, fixed_source_energy);

    double w_source = sum_weights(state.current_bank);

    if (w_source <= 0.0 || !std::isfinite(w_source))
      throw std::runtime_error(
          "Source bank extinction: non-positive source weight");

    state.tallies.begin_cycle(tally_on);
    transport_cycle(state);
    state.tallies.end_cycle();

    state.fission_bank.clear_active();
    state.fission_count = 0;
  }

  auto end = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "\nFixed-source calculation completed in: "
            << elapsed.count() / 1000.0 << " seconds.\n";

  if (tally_on) {
    RunMetadata meta;
    meta.mode = "fixed_source";
    meta.n_particles = n_particles;
    meta.n_inactive = 0;
    meta.n_active = n_active;
    meta.seed = seed;
    meta.material_name = std::string(mat.name);
    meta.temperature_K = mat.temperature;

    TallyOutputOptions out;
    out.file_path = "tallies/fixed_source_tallies.json";
    state.tallies.write_json(meta, mat, data, out);
  }
}
