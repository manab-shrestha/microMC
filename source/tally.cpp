#include "tally.h"

#include "xs_lookup.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>

namespace {

std::string tally_quantity_name(TallyQuantity q) {
  switch (q) {
  case TallyQuantity::FLUX:
    return "FLUX";
  case TallyQuantity::RXN_RATE:
    return "RXN_RATE";
  case TallyQuantity::NU_FISSION:
    return "NU_FISSION";
  case TallyQuantity::KAPPA_FISSION:
    return "KAPPA_FISSION";
  }
  return "UNKNOWN";
}

std::string grid_dim_name(GridDim d) {
  switch (d) {
  case GridDim::ENERGY:
    return "ENERGY";
  case GridDim::X:
    return "X";
  case GridDim::Y:
    return "Y";
  case GridDim::Z:
    return "Z";
  }
  return "UNKNOWN";
}

std::string grid_spacing_name(GridSpacing s) {
  switch (s) {
  case GridSpacing::CUSTOM_EDGES:
    return "CUSTOM_EDGES";
  case GridSpacing::UNIFORM_LINEAR:
    return "UNIFORM_LINEAR";
  case GridSpacing::UNIFORM_LETHARGY:
    return "UNIFORM_LETHARGY";
  case GridSpacing::UNIFORM_LOG10:
    return "UNIFORM_LOG10";
  }
  return "UNKNOWN";
}

std::string outside_policy_name(GridOutsidePolicy p) {
  switch (p) {
  case GridOutsidePolicy::DROP:
    return "DROP";
  case GridOutsidePolicy::CLAMP:
    return "CLAMP";
  }
  return "UNKNOWN";
}

std::string json_escape(const std::string &s) {
  std::string out;
  out.reserve(s.size() + 8);
  for (char c : s) {
    switch (c) {
    case '"':
      out += "\\\"";
      break;
    case '\\':
      out += "\\\\";
      break;
    case '\n':
      out += "\\n";
      break;
    case '\r':
      out += "\\r";
      break;
    case '\t':
      out += "\\t";
      break;
    default:
      out += c;
      break;
    }
  }
  return out;
}

bool is_strictly_increasing(const std::vector<double> &v) {
  if (v.size() < 2)
    return false;
  for (size_t i = 1; i < v.size(); ++i) {
    if (!(v[i] > v[i - 1]) || !std::isfinite(v[i]))
      return false;
  }
  return std::isfinite(v.front());
}

std::vector<double> build_grid_edges(const GridDimSpec &dim,
                                     const std::string &tally_name) {
  std::vector<double> edges;

  switch (dim.spacing) {
  case GridSpacing::CUSTOM_EDGES:
    edges = dim.bin_edges;
    if (!is_strictly_increasing(edges)) {
      throw std::runtime_error("Tally '" + tally_name +
                               "': custom grid edges must be strictly "
                               "increasing and finite");
    }
    return edges;

  case GridSpacing::UNIFORM_LINEAR: {
    if (dim.n_bins <= 0 || !(dim.max > dim.min) ||
        !std::isfinite(dim.min) || !std::isfinite(dim.max)) {
      throw std::runtime_error("Tally '" + tally_name +
                               "': invalid UNIFORM_LINEAR grid parameters");
    }
    edges.resize(static_cast<size_t>(dim.n_bins) + 1);
    const double dx = (dim.max - dim.min) / dim.n_bins;
    for (int i = 0; i <= dim.n_bins; ++i)
      edges[static_cast<size_t>(i)] = dim.min + i * dx;
    return edges;
  }

  case GridSpacing::UNIFORM_LETHARGY: {
    if (dim.n_bins <= 0 || !(dim.max > dim.min) || dim.min <= 0.0 ||
        !std::isfinite(dim.min) || !std::isfinite(dim.max)) {
      throw std::runtime_error("Tally '" + tally_name +
                               "': invalid UNIFORM_LETHARGY grid parameters");
    }
    edges.resize(static_cast<size_t>(dim.n_bins) + 1);
    const double du = std::log(dim.max / dim.min) / dim.n_bins;
    for (int i = 0; i <= dim.n_bins; ++i)
      edges[static_cast<size_t>(i)] = dim.min * std::exp(i * du);
    return edges;
  }

  case GridSpacing::UNIFORM_LOG10: {
    if (dim.n_bins <= 0 || !(dim.max > dim.min) || dim.min <= 0.0 ||
        !std::isfinite(dim.min) || !std::isfinite(dim.max)) {
      throw std::runtime_error("Tally '" + tally_name +
                               "': invalid UNIFORM_LOG10 grid parameters");
    }
    edges.resize(static_cast<size_t>(dim.n_bins) + 1);
    const double lmin = std::log10(dim.min);
    const double lmax = std::log10(dim.max);
    const double dl = (lmax - lmin) / dim.n_bins;
    for (int i = 0; i <= dim.n_bins; ++i)
      edges[static_cast<size_t>(i)] = std::pow(10.0, lmin + i * dl);
    return edges;
  }
  }

  throw std::runtime_error("Unsupported grid spacing mode");
}

int locate_grid_bin(const std::vector<double> &edges, double value,
                    GridOutsidePolicy policy) {
  const int n_bins = static_cast<int>(edges.size()) - 1;
  if (n_bins <= 0 || !std::isfinite(value))
    return -1;

  if (value < edges.front())
    return (policy == GridOutsidePolicy::CLAMP) ? 0 : -1;

  if (value >= edges.back())
    return (policy == GridOutsidePolicy::CLAMP) ? (n_bins - 1) : -1;

  auto it = std::upper_bound(edges.begin(), edges.end(), value);
  int bin = static_cast<int>(it - edges.begin()) - 1;
  if (bin < 0 || bin >= n_bins)
    return -1;
  return bin;
}

double value_for_dim(GridDim dim, double x, double y, double z, double E) {
  switch (dim) {
  case GridDim::ENERGY:
    return E;
  case GridDim::X:
    return x;
  case GridDim::Y:
    return y;
  case GridDim::Z:
    return z;
  }
  return std::numeric_limits<double>::quiet_NaN();
}

int locate_tally_bin(const TallySpec &spec,
                     const std::vector<std::vector<double>> &edges_by_dim,
                     const std::vector<int> &shape,
                     const std::vector<GridOutsidePolicy> &policies, double x,
                     double y, double z, double E) {
  int flat = 0;
  for (size_t d = 0; d < spec.grid.dims.size(); ++d) {
    const GridDimSpec &dim = spec.grid.dims[d];
    const double value = value_for_dim(dim.dim, x, y, z, E);
    const int bin = locate_grid_bin(edges_by_dim[d], value, policies[d]);
    if (bin < 0)
      return -1;
    flat = flat * shape[d] + bin;
  }
  return flat;
}

int n_total_bins(const std::vector<int> &shape) {
  int total = 1;
  for (int n : shape) {
    if (n <= 0)
      return 0;
    if (total > std::numeric_limits<int>::max() / n)
      throw std::runtime_error("Tally grid has too many bins");
    total *= n;
  }
  return total;
}

bool rxn_in_filter(RxnType type, const std::vector<RxnType> &filter) {
  if (filter.empty())
    return true;
  for (RxnType t : filter) {
    if (t == type)
      return true;
  }
  return false;
}

double interp_nu_bar_deterministic(const FissionYieldPool &pool, int yield_id,
                                   double E) {
  if (yield_id < 0 || yield_id >= pool.n_yields)
    return 0.0;

  const FissionYieldDescriptor &desc = pool.descriptors[yield_id];
  if (desc.n_points <= 0)
    return 0.0;

  const int off = desc.offset;
  const int n = desc.n_points;
  const double *E_tab = pool.energy + off;
  const double *nu_tab = pool.nu_bar + off;

  if (n == 1)
    return nu_tab[0];
  if (E <= E_tab[0])
    return nu_tab[0];
  if (E >= E_tab[n - 1])
    return nu_tab[n - 1];

  int lo = 0;
  int hi = n - 1;
  while (lo < hi) {
    int mid = lo + (hi - lo + 1) / 2;
    if (E_tab[mid] <= E)
      lo = mid;
    else
      hi = mid - 1;
  }

  int bin = lo;
  if (bin >= n - 1)
    bin = n - 2;
  const double Elo = E_tab[bin];
  const double Ehi = E_tab[bin + 1];
  if (!(Ehi > Elo))
    return nu_tab[bin];

  const double frac = std::clamp((E - Elo) / (Ehi - Elo), 0.0, 1.0);
  return nu_tab[bin] + frac * (nu_tab[bin + 1] - nu_tab[bin]);
}

double eval_kappa(const NuclideDescriptor &nuc, const NuclearData &data,
                  double E) {
  if (nuc.kappa_id < 0 || nuc.kappa_id >= data.n_kappa_descs)
    return 0.0;
  const KappaDescriptor &k = data.kappa_descs[nuc.kappa_id];
  return k.coeffs[0] + k.coeffs[1] * E + k.coeffs[2] * E * E;
}

double macro_total_filtered(const Material &mat, const NuclearData &data,
                            double E,
                            const std::vector<int> &selected_mat_slots) {
  double sum = 0.0;
  for (int slot : selected_mat_slots) {
    const int nuclide_idx = mat.nuclide_ids[slot];
    const double N = mat.number_densities[slot];
    if (N <= 0.0)
      continue;
    const NuclideDescriptor &nuc = data.nuclides[nuclide_idx];
    const GridIndex gi = find_grid_index(data, nuc, E);
    for (int r = 0; r < nuc.n_reactions; ++r) {
      const ReactionDescriptor &rxn = data.reactions[nuc.rxn_offset + r];
      const double micro = lookup_micro_xs_at(data, rxn, gi);
      if (micro > 0.0)
        sum += N * micro;
    }
  }
  return sum;
}

double macro_rxn_rate_filtered(const Material &mat, const NuclearData &data,
                               double E,
                               const std::vector<int> &selected_mat_slots,
                               const std::vector<RxnType> &rxn_filter) {
  double sum = 0.0;
  for (int slot : selected_mat_slots) {
    const int nuclide_idx = mat.nuclide_ids[slot];
    const double N = mat.number_densities[slot];
    if (N <= 0.0)
      continue;
    const NuclideDescriptor &nuc = data.nuclides[nuclide_idx];
    const GridIndex gi = find_grid_index(data, nuc, E);
    for (int r = 0; r < nuc.n_reactions; ++r) {
      const ReactionDescriptor &rxn = data.reactions[nuc.rxn_offset + r];
      if (!rxn_in_filter(rxn.type, rxn_filter))
        continue;
      const double micro = lookup_micro_xs_at(data, rxn, gi);
      if (micro > 0.0)
        sum += N * micro;
    }
  }
  return sum;
}

double macro_nu_fission_filtered(const Material &mat, const NuclearData &data,
                                 double E,
                                 const std::vector<int> &selected_mat_slots) {
  double sum = 0.0;
  for (int slot : selected_mat_slots) {
    const int nuclide_idx = mat.nuclide_ids[slot];
    const double N = mat.number_densities[slot];
    if (N <= 0.0)
      continue;
    const NuclideDescriptor &nuc = data.nuclides[nuclide_idx];
    const GridIndex gi = find_grid_index(data, nuc, E);
    for (int r = 0; r < nuc.n_reactions; ++r) {
      const ReactionDescriptor &rxn = data.reactions[nuc.rxn_offset + r];
      if (rxn.type != RxnType::FISSION)
        continue;
      const double micro_f = lookup_micro_xs_at(data, rxn, gi);
      if (micro_f <= 0.0)
        continue;
      const double nu_bar =
          interp_nu_bar_deterministic(data.fission_yields, rxn.yield_id, E);
      sum += N * nu_bar * micro_f;
    }
  }
  return sum;
}

double macro_kappa_fission_filtered(const Material &mat, const NuclearData &data,
                                    double E,
                                    const std::vector<int> &selected_mat_slots) {
  double sum = 0.0;
  for (int slot : selected_mat_slots) {
    const int nuclide_idx = mat.nuclide_ids[slot];
    const double N = mat.number_densities[slot];
    if (N <= 0.0)
      continue;
    const NuclideDescriptor &nuc = data.nuclides[nuclide_idx];
    const double kappa = eval_kappa(nuc, data, E);
    if (kappa == 0.0)
      continue;
    const GridIndex gi = find_grid_index(data, nuc, E);
    for (int r = 0; r < nuc.n_reactions; ++r) {
      const ReactionDescriptor &rxn = data.reactions[nuc.rxn_offset + r];
      if (rxn.type != RxnType::FISSION)
        continue;
      const double micro_f = lookup_micro_xs_at(data, rxn, gi);
      if (micro_f > 0.0)
        sum += N * kappa * micro_f;
    }
  }
  return sum;
}

bool is_all_nuclides_selected(const std::vector<int> &slots,
                              int n_material_nuclides) {
  return static_cast<int>(slots.size()) == n_material_nuclides;
}

std::vector<int> resolve_nuclide_slots(const NuclideFilter &filter,
                                       const Material &mat,
                                       const std::string &tally_name) {
  std::vector<int> slots;

  if (filter.zaids.empty()) {
    slots.reserve(mat.n_nuclides);
    for (int i = 0; i < mat.n_nuclides; ++i)
      slots.push_back(i);
    return slots;
  }

  for (int req_zaid : filter.zaids) {
    bool found = false;
    for (int i = 0; i < mat.n_nuclides; ++i) {
      if (mat.zaids[i] == req_zaid) {
        if (std::find(slots.begin(), slots.end(), i) == slots.end())
          slots.push_back(i);
        found = true;
        break;
      }
    }
    if (!found) {
      throw std::runtime_error("Tally '" + tally_name + "': ZAID " +
                               std::to_string(req_zaid) +
                               " is not present in material '" +
                               std::string(mat.name) + "'");
    }
  }

  return slots;
}

bool material_matches(const MaterialFilter &filter, const Material &mat) {
  if (filter.names.empty())
    return true;
  const std::string mname(mat.name);
  for (const std::string &name : filter.names) {
    if (name == mname)
      return true;
  }
  return false;
}

void write_indent(std::ostream &os, int level, int step, bool pretty) {
  if (!pretty)
    return;
  for (int i = 0; i < level * step; ++i)
    os.put(' ');
}

void write_newline(std::ostream &os, bool pretty) {
  if (pretty)
    os.put('\n');
}

template <typename T>
void write_number_array(std::ostream &os, const std::vector<T> &values,
                        int level, int step, bool pretty) {
  os << '[';
  if (!values.empty()) {
    write_newline(os, pretty);
    for (size_t i = 0; i < values.size(); ++i) {
      write_indent(os, level + 1, step, pretty);
      os << values[i];
      if (i + 1 != values.size())
        os << ',';
      write_newline(os, pretty);
    }
    write_indent(os, level, step, pretty);
  }
  os << ']';
}

void write_string_array(std::ostream &os, const std::vector<std::string> &values,
                        int level, int step, bool pretty) {
  os << '[';
  if (!values.empty()) {
    write_newline(os, pretty);
    for (size_t i = 0; i < values.size(); ++i) {
      write_indent(os, level + 1, step, pretty);
      os << '"' << json_escape(values[i]) << '"';
      if (i + 1 != values.size())
        os << ',';
      write_newline(os, pretty);
    }
    write_indent(os, level, step, pretty);
  }
  os << ']';
}

} // namespace

void TallyManager::configure(const Material &mat, const NuclearData &data,
                             const std::vector<TallySpec> &specs) {
  (void)data;

  specs_ = specs;
  n_active_cycles_ = 0;
  scoring_active_ = false;

  grid_edges_.clear();
  grid_shapes_.clear();
  outside_policies_.clear();
  material_enabled_.clear();
  selected_mat_slots_.clear();
  rxn_filters_.clear();
  stats_.clear();
  mean_cache_.clear();
  std_err_cache_.clear();
  rel_err_cache_.clear();

  const size_t n_tallies = specs_.size();
  grid_edges_.resize(n_tallies);
  grid_shapes_.resize(n_tallies);
  outside_policies_.resize(n_tallies);
  material_enabled_.resize(n_tallies, true);
  selected_mat_slots_.resize(n_tallies);
  rxn_filters_.resize(n_tallies);
  stats_.resize(n_tallies);
  mean_cache_.resize(n_tallies);
  std_err_cache_.resize(n_tallies);
  rel_err_cache_.resize(n_tallies);

  std::set<std::string> names;

  for (size_t t = 0; t < n_tallies; ++t) {
    const TallySpec &spec = specs_[t];
    if (spec.name.empty())
      throw std::runtime_error("Tally name must not be empty");
    if (!names.insert(spec.name).second)
      throw std::runtime_error("Duplicate tally name: '" + spec.name + "'");

    if (spec.grid.dims.empty()) {
      throw std::runtime_error("Tally '" + spec.name +
                               "': requires at least one grid dimension");
    }

    std::set<int> dims_seen;
    grid_edges_[t].reserve(spec.grid.dims.size());
    grid_shapes_[t].reserve(spec.grid.dims.size());
    outside_policies_[t].reserve(spec.grid.dims.size());

    for (const GridDimSpec &dim : spec.grid.dims) {
      const int dim_key = static_cast<int>(dim.dim);
      if (!dims_seen.insert(dim_key).second) {
        throw std::runtime_error("Tally '" + spec.name +
                                 "': duplicate grid dimension '" +
                                 grid_dim_name(dim.dim) + "'");
      }

      grid_edges_[t].push_back(build_grid_edges(dim, spec.name));
      grid_shapes_[t].push_back(
          static_cast<int>(grid_edges_[t].back().size()) - 1);
      outside_policies_[t].push_back(dim.outside_policy);
    }

    const int total_bins = n_total_bins(grid_shapes_[t]);

    material_enabled_[t] = material_matches(spec.materials, mat);
    selected_mat_slots_[t] =
        resolve_nuclide_slots(spec.nuclides, mat, spec.name);

    if (spec.quantity != TallyQuantity::RXN_RATE &&
        !spec.reactions.types.empty()) {
      throw std::runtime_error("Tally '" + spec.name +
                               "': reaction filter is only valid for "
                               "RXN_RATE tallies");
    }

    rxn_filters_[t] = spec.reactions.types;

    TallyBinStats &s = stats_[t];
    s.sum.assign(static_cast<size_t>(total_bins), 0.0);
    s.sum_sq.assign(static_cast<size_t>(total_bins), 0.0);
    s.cycle_sum.assign(static_cast<size_t>(total_bins), 0.0);

    mean_cache_[t].assign(static_cast<size_t>(total_bins), 0.0);
    std_err_cache_[t].assign(static_cast<size_t>(total_bins), 0.0);
    rel_err_cache_[t].assign(static_cast<size_t>(total_bins), 0.0);
  }
}

void TallyManager::begin_cycle(bool active_cycle) {
  scoring_active_ = active_cycle;
  if (!scoring_active_)
    return;

  for (TallyBinStats &s : stats_)
    std::fill(s.cycle_sum.begin(), s.cycle_sum.end(), 0.0);
}

void TallyManager::score_collision(double x, double y, double z, double E,
                                   double w, double macro_xs_t,
                                   const Material &mat,
                                   const NuclearData &data) {
  if (!scoring_active_)
    return;
  if (!(macro_xs_t > 0.0) || !std::isfinite(macro_xs_t) || !std::isfinite(w) ||
      !std::isfinite(E) || !std::isfinite(x) || !std::isfinite(y) ||
      !std::isfinite(z)) {
    return;
  }

  for (size_t t = 0; t < specs_.size(); ++t) {
    if (!material_enabled_[t])
      continue;

    const int bin = locate_tally_bin(specs_[t], grid_edges_[t], grid_shapes_[t],
                                     outside_policies_[t], x, y, z, E);
    if (bin < 0)
      continue;

    const std::vector<int> &slots = selected_mat_slots_[t];
    if (slots.empty())
      continue;

    double multiplier = 0.0;
    const TallySpec &spec = specs_[t];

    switch (spec.quantity) {
    case TallyQuantity::FLUX: {
      if (is_all_nuclides_selected(slots, mat.n_nuclides)) {
        multiplier = 1.0;
      } else {
        const double sigma_t_filtered =
            macro_total_filtered(mat, data, E, slots);
        multiplier = sigma_t_filtered / macro_xs_t;
        multiplier = std::clamp(multiplier, 0.0, 1.0);
      }
      break;
    }
    case TallyQuantity::RXN_RATE:
      multiplier = macro_rxn_rate_filtered(mat, data, E, slots, rxn_filters_[t]);
      break;
    case TallyQuantity::NU_FISSION:
      multiplier = macro_nu_fission_filtered(mat, data, E, slots);
      break;
    case TallyQuantity::KAPPA_FISSION:
      multiplier = macro_kappa_fission_filtered(mat, data, E, slots);
      break;
    }

    if (!(multiplier > 0.0) || !std::isfinite(multiplier))
      continue;

    const double score = w * multiplier / macro_xs_t;
    if (!std::isfinite(score))
      continue;

    stats_[t].cycle_sum[static_cast<size_t>(bin)] += score;
  }
}

void TallyManager::end_cycle() {
  if (!scoring_active_)
    return;

  for (TallyBinStats &s : stats_) {
    for (size_t i = 0; i < s.cycle_sum.size(); ++i) {
      const double v = s.cycle_sum[i];
      s.sum[i] += v;
      s.sum_sq[i] += v * v;
    }
  }

  ++n_active_cycles_;
}

int TallyManager::n_active_cycles() const { return n_active_cycles_; }

const std::vector<TallySpec> &TallyManager::specs() const { return specs_; }

std::vector<TallyResultView> TallyManager::results() const {
  std::vector<TallyResultView> out;
  out.reserve(specs_.size());

  const int n = n_active_cycles_;
  for (size_t t = 0; t < specs_.size(); ++t) {
    const TallyBinStats &s = stats_[t];
    std::vector<double> &mean = mean_cache_[t];
    std::vector<double> &std_err = std_err_cache_[t];
    std::vector<double> &rel_err = rel_err_cache_[t];

    const size_t n_bins = s.sum.size();
    mean.assign(n_bins, 0.0);
    std_err.assign(n_bins, 0.0);
    rel_err.assign(n_bins, 0.0);

    if (n > 0) {
      for (size_t i = 0; i < n_bins; ++i)
        mean[i] = s.sum[i] / n;
    }

    if (n > 1) {
      for (size_t i = 0; i < n_bins; ++i) {
        const double m = mean[i];
        const double ex2 = s.sum_sq[i] / n;
        const double sample_var = std::max(0.0, (ex2 - m * m) * n / (n - 1));
        const double se = std::sqrt(sample_var / n);
        std_err[i] = se;
        rel_err[i] = (std::abs(m) > 0.0) ? (se / std::abs(m)) : 0.0;
      }
    }

    TallyResultView view;
    view.name = specs_[t].name;
    view.quantity = specs_[t].quantity;
    view.grid_shape = grid_shapes_[t];
    view.mean = &mean;
    view.std_err = &std_err;
    view.rel_err = &rel_err;
    out.push_back(std::move(view));
  }

  return out;
}

void TallyManager::write_json(const RunMetadata &run_meta, const Material &mat,
                              const NuclearData &data,
                              const TallyOutputOptions &options) const {
  (void)data;

  const bool pretty = options.pretty;
  const int indent = (options.indent > 0) ? options.indent : 2;

  std::filesystem::path out_path(options.file_path);
  if (out_path.has_parent_path())
    std::filesystem::create_directories(out_path.parent_path());

  std::ofstream os(options.file_path);
  if (!os)
    throw std::runtime_error("Failed to open tally JSON file: " +
                             options.file_path);

  os << std::setprecision(17);

  os << '{';
  write_newline(os, pretty);

  write_indent(os, 1, indent, pretty);
  os << "\"run_metadata\":";
  if (pretty)
    os << ' ';
  os << '{';
  write_newline(os, pretty);

  write_indent(os, 2, indent, pretty);
  os << "\"mode\": \"" << json_escape(run_meta.mode) << "\",";
  write_newline(os, pretty);
  write_indent(os, 2, indent, pretty);
  os << "\"n_particles\": " << run_meta.n_particles << ',';
  write_newline(os, pretty);
  write_indent(os, 2, indent, pretty);
  os << "\"n_inactive\": " << run_meta.n_inactive << ',';
  write_newline(os, pretty);
  write_indent(os, 2, indent, pretty);
  os << "\"n_active\": " << run_meta.n_active << ',';
  write_newline(os, pretty);
  write_indent(os, 2, indent, pretty);
  os << "\"seed\": " << run_meta.seed << ',';
  write_newline(os, pretty);
  write_indent(os, 2, indent, pretty);
  os << "\"material_name\": \"" << json_escape(run_meta.material_name)
     << "\",";
  write_newline(os, pretty);
  write_indent(os, 2, indent, pretty);
  os << "\"temperature_K\": " << run_meta.temperature_K << ',';
  write_newline(os, pretty);

  write_indent(os, 2, indent, pretty);
  os << "\"k_eff_active_history\": ";
  write_number_array(os, run_meta.k_eff_active_history, 2, indent, pretty);
  os << ',';
  write_newline(os, pretty);

  write_indent(os, 2, indent, pretty);
  os << "\"k_eff_active_rel_err_history\": ";
  write_number_array(os, run_meta.k_eff_active_rel_err_history, 2, indent,
                     pretty);
  os << ',';
  write_newline(os, pretty);

  write_indent(os, 2, indent, pretty);
  os << "\"k_eff_final_mean\": " << run_meta.k_eff_final_mean << ',';
  write_newline(os, pretty);

  write_indent(os, 2, indent, pretty);
  os << "\"k_eff_final_rel_err\": " << run_meta.k_eff_final_rel_err;
  write_newline(os, pretty);

  write_indent(os, 1, indent, pretty);
  os << "},";
  write_newline(os, pretty);

  write_indent(os, 1, indent, pretty);
  os << "\"tallies\":";
  if (pretty)
    os << ' ';
  os << '[';
  write_newline(os, pretty);

  const auto views = results();
  for (size_t t = 0; t < specs_.size(); ++t) {
    const TallySpec &spec = specs_[t];
    const TallyResultView &view = views[t];

    write_indent(os, 2, indent, pretty);
    os << '{';
    write_newline(os, pretty);

    write_indent(os, 3, indent, pretty);
    os << "\"name\": \"" << json_escape(spec.name) << "\",";
    write_newline(os, pretty);

    write_indent(os, 3, indent, pretty);
    os << "\"quantity\": \"" << tally_quantity_name(spec.quantity)
       << "\",";
    write_newline(os, pretty);

    write_indent(os, 3, indent, pretty);
    os << "\"material_filter_names\": ";
    write_string_array(os, spec.materials.names, 3, indent, pretty);
    os << ',';
    write_newline(os, pretty);

    write_indent(os, 3, indent, pretty);
    os << "\"nuclide_filter_zaids\": ";
    write_number_array(os, spec.nuclides.zaids, 3, indent, pretty);
    os << ',';
    write_newline(os, pretty);

    std::vector<std::string> rxn_names;
    rxn_names.reserve(spec.reactions.types.size());
    for (RxnType rt : spec.reactions.types)
      rxn_names.emplace_back(rxn_type_name(rt));
    write_indent(os, 3, indent, pretty);
    os << "\"reaction_filter\": ";
    write_string_array(os, rxn_names, 3, indent, pretty);
    os << ',';
    write_newline(os, pretty);

    write_indent(os, 3, indent, pretty);
    os << "\"grid\": ";
    os << '{';
    write_newline(os, pretty);

    write_indent(os, 4, indent, pretty);
    os << "\"dims\": [";
    write_newline(os, pretty);

    for (size_t d = 0; d < spec.grid.dims.size(); ++d) {
      const GridDimSpec &dim = spec.grid.dims[d];
      write_indent(os, 5, indent, pretty);
      os << '{';
      write_newline(os, pretty);

      write_indent(os, 6, indent, pretty);
      os << "\"dim\": \"" << grid_dim_name(dim.dim) << "\",";
      write_newline(os, pretty);

      write_indent(os, 6, indent, pretty);
      os << "\"spacing\": \"" << grid_spacing_name(dim.spacing) << "\",";
      write_newline(os, pretty);

      write_indent(os, 6, indent, pretty);
      os << "\"outside_policy\": \""
         << outside_policy_name(dim.outside_policy) << "\",";
      write_newline(os, pretty);

      write_indent(os, 6, indent, pretty);
      os << "\"bin_edges\": ";
      write_number_array(os, grid_edges_[t][d], 6, indent, pretty);
      write_newline(os, pretty);

      write_indent(os, 5, indent, pretty);
      os << '}';
      if (d + 1 != spec.grid.dims.size())
        os << ',';
      write_newline(os, pretty);
    }

    write_indent(os, 4, indent, pretty);
    os << "],";
    write_newline(os, pretty);

    write_indent(os, 4, indent, pretty);
    os << "\"grid_shape\": ";
    write_number_array(os, grid_shapes_[t], 4, indent, pretty);
    write_newline(os, pretty);

    write_indent(os, 3, indent, pretty);
    os << "},";
    write_newline(os, pretty);

    write_indent(os, 3, indent, pretty);
    os << "\"results\": ";
    os << '{';
    write_newline(os, pretty);

    write_indent(os, 4, indent, pretty);
    os << "\"mean\": ";
    write_number_array(os, *view.mean, 4, indent, pretty);
    os << ',';
    write_newline(os, pretty);

    write_indent(os, 4, indent, pretty);
    os << "\"std_err\": ";
    write_number_array(os, *view.std_err, 4, indent, pretty);
    os << ',';
    write_newline(os, pretty);

    write_indent(os, 4, indent, pretty);
    os << "\"rel_err\": ";
    write_number_array(os, *view.rel_err, 4, indent, pretty);

    if (options.include_sum_and_sum_sq) {
      os << ',';
      write_newline(os, pretty);

      write_indent(os, 4, indent, pretty);
      os << "\"sum\": ";
      write_number_array(os, stats_[t].sum, 4, indent, pretty);
      os << ',';
      write_newline(os, pretty);

      write_indent(os, 4, indent, pretty);
      os << "\"sum_sq\": ";
      write_number_array(os, stats_[t].sum_sq, 4, indent, pretty);
    }

    os << ',';
    write_newline(os, pretty);
    write_indent(os, 4, indent, pretty);
    os << "\"n_active_cycles\": " << n_active_cycles_;
    write_newline(os, pretty);

    write_indent(os, 3, indent, pretty);
    os << '}';
    write_newline(os, pretty);

    write_indent(os, 2, indent, pretty);
    os << '}';
    if (t + 1 != specs_.size())
      os << ',';
    write_newline(os, pretty);
  }

  write_indent(os, 1, indent, pretty);
  os << ']';
  write_newline(os, pretty);

  write_indent(os, 0, indent, pretty);
  os << '}';
  write_newline(os, pretty);

  (void)mat;
}
