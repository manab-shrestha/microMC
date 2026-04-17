#pragma once

#include "material.h"
#include "neutron.h"
#include "nuclear_data.h"

#include <cstdint>
#include <string>
#include <vector>

enum class TallyQuantity : int {
  FLUX = 0,
  RXN_RATE = 1,
  NU_FISSION = 2,
  KAPPA_FISSION = 3,
};

enum class GridDim : int {
  ENERGY = 0,
};

enum class GridSpacing : int {
  CUSTOM_EDGES = 0,
  UNIFORM_LINEAR = 1,
  UNIFORM_LETHARGY = 2,
  UNIFORM_LOG10 = 3,
};

enum class GridOutsidePolicy : int {
  DROP = 0,
  CLAMP = 1,
};

struct NuclideFilter {
  // Empty means all nuclides in the current material.
  std::vector<int> zaids;
};

struct MaterialFilter {
  // Empty means all materials.
  std::vector<std::string> names;
};

struct ReactionFilter {
  // Only used for RXN_RATE tallies. Empty means all reaction types.
  std::vector<RxnType> types;
};

struct GridDimSpec {
  GridDim dim{GridDim::ENERGY};
  GridSpacing spacing{GridSpacing::CUSTOM_EDGES};

  // CUSTOM_EDGES mode
  std::vector<double> bin_edges_eV;

  // Generated grid modes (UNIFORM_LINEAR, UNIFORM_LETHARGY, UNIFORM_LOG10)
  double min_eV{0.0};
  double max_eV{0.0};
  int n_bins{0};

  GridOutsidePolicy outside_policy{GridOutsidePolicy::DROP};
};

struct TallyGridSpec {
  std::vector<GridDimSpec> dims;
};

struct TallySpec {
  std::string name;
  TallyQuantity quantity{TallyQuantity::FLUX};
  MaterialFilter materials{};
  NuclideFilter nuclides{};
  ReactionFilter reactions{};
  TallyGridSpec grid;
};

struct TallyBinStats {
  std::vector<double> sum;
  std::vector<double> sum_sq;
  std::vector<double> cycle_sum;
};

struct TallyResultView {
  std::string name;
  TallyQuantity quantity{TallyQuantity::FLUX};
  std::vector<int> grid_shape;
  const std::vector<double> *mean{nullptr};
  const std::vector<double> *std_err{nullptr};
  const std::vector<double> *rel_err{nullptr};
};

struct RunMetadata {
  std::string mode;
  int n_particles{0};
  int n_inactive{0};
  int n_active{0};
  uint64_t seed{0};
  std::string material_name;
  double temperature_K{0.0};
  std::vector<double> k_eff_active_history;
  std::vector<double> k_eff_active_rel_err_history;
  double k_eff_final_mean{0.0};
  double k_eff_final_rel_err{0.0};
};

struct TallyOutputOptions {
  std::string file_path{"tallies/results.json"};
  bool pretty{true};
  int indent{2};
  bool include_sum_and_sum_sq{true};
};

class TallyManager {
public:
  void configure(const Material &mat, const NuclearData &data,
                 const std::vector<TallySpec> &specs);

  void begin_cycle(bool active_cycle);
  void score_collision(const Neutron &neutron, const Material &mat,
                       const NuclearData &data);
  void end_cycle();

  int n_active_cycles() const;
  const std::vector<TallySpec> &specs() const;
  std::vector<TallyResultView> results() const;

  void write_json(const RunMetadata &run_meta, const Material &mat,
                  const NuclearData &data,
                  const TallyOutputOptions &options) const;

private:
  std::vector<TallySpec> specs_;
  std::vector<std::vector<double>> grid_edges_eV_;
  std::vector<int> grid_n_bins_;
  std::vector<std::vector<int>> grid_shapes_;
  std::vector<GridOutsidePolicy> outside_policies_;
  std::vector<bool> material_enabled_;
  std::vector<std::vector<int>> selected_mat_slots_;
  std::vector<std::vector<RxnType>> rxn_filters_;
  std::vector<TallyBinStats> stats_;
  mutable std::vector<std::vector<double>> mean_cache_;
  mutable std::vector<std::vector<double>> std_err_cache_;
  mutable std::vector<std::vector<double>> rel_err_cache_;
  int n_active_cycles_{0};
  bool scoring_active_{false};
};
