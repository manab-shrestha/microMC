#include "material.h"
#include "nuclear_data.h"
#include <H5Cpp.h>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
namespace fs = std::filesystem;

// ── HDF5 helper functions ────────────────────────────────────────────

namespace {

double read_double_attr(const H5::H5Object &obj, const std::string &name) {
  H5::Attribute attr = obj.openAttribute(name);
  double val;
  attr.read(H5::PredType::NATIVE_DOUBLE, &val);
  return val;
}

int read_int_attr(const H5::H5Object &obj, const std::string &name) {
  H5::Attribute attr = obj.openAttribute(name);
  long long val;
  attr.read(H5::PredType::NATIVE_LLONG, &val);
  return static_cast<int>(val);
}

std::string read_string_attr(const H5::H5Object &obj, const std::string &name) {
  H5::Attribute attr = obj.openAttribute(name);
  std::string val;
  H5::DataType dtype = attr.getDataType();
  if (dtype.isVariableStr()) {
    attr.read(dtype, val);
  } else {
    size_t sz = dtype.getSize();
    std::vector<char> buf(sz + 1, '\0');
    attr.read(dtype, buf.data());
    val = std::string(buf.data());
    // Trim null padding
    auto pos = val.find('\0');
    if (pos != std::string::npos)
      val.resize(pos);
  }
  return val;
}

bool has_attr(const H5::H5Object &obj, const std::string &name) {
  return obj.attrExists(name);
}

std::vector<double> read_1d_double(H5::DataSet &ds) {
  H5::DataSpace space = ds.getSpace();
  hsize_t dim;
  space.getSimpleExtentDims(&dim);
  std::vector<double> v(dim);
  ds.read(v.data(), H5::PredType::NATIVE_DOUBLE);
  return v;
}

// Read a 2D dataset into a flat row-major buffer. Returns dims via out params.
std::vector<double> read_2d_double(H5::DataSet &ds, int &rows, int &cols) {
  H5::DataSpace space = ds.getSpace();
  hsize_t dims[2];
  space.getSimpleExtentDims(dims);
  rows = static_cast<int>(dims[0]);
  cols = static_cast<int>(dims[1]);
  std::vector<double> buf(rows * cols);
  ds.read(buf.data(), H5::PredType::NATIVE_DOUBLE);
  return buf;
}

// Read an int64 attribute that is a 1D array (e.g. offsets).
std::vector<int> read_1d_int_attr(const H5::Attribute &attr) {
  H5::DataSpace space = attr.getSpace();
  hsize_t dim;
  space.getSimpleExtentDims(&dim);
  std::vector<long long> buf(dim);
  attr.read(H5::PredType::NATIVE_LLONG, buf.data());
  std::vector<int> result(dim);
  for (hsize_t i = 0; i < dim; ++i)
    result[i] = static_cast<int>(buf[i]);
  return result;
}

// Check if a group contains a named child (group or dataset).
bool has_child(const H5::Group &g, const std::string &name) {
  return g.nameExists(name);
}

// Enumerate child names of a group.
std::vector<std::string> child_names(const H5::Group &g) {
  std::vector<std::string> names;
  hsize_t n = g.getNumObjs();
  for (hsize_t i = 0; i < n; ++i)
    names.push_back(g.getObjnameByIdx(i));
  return names;
}

// ── Pool builder structs ─────────────────────────────────────────────

struct AngularPoolBuilder {
  std::vector<int> energy_offsets = {0};
  std::vector<double> energies;
  std::vector<int> table_offsets = {0};
  std::vector<double> mu, pdf, cdf;
  int n_dists = 0;

  int add(H5::Group &angle_group) {
    int dist_id = n_dists++;

    H5::DataSet e_ds = angle_group.openDataSet("energy");
    auto E = read_1d_double(e_ds);
    energies.insert(energies.end(), E.begin(), E.end());
    energy_offsets.push_back(static_cast<int>(energies.size()));

    H5::DataSet mu_ds = angle_group.openDataSet("mu");
    int rows, cols;
    auto mu_data = read_2d_double(mu_ds, rows, cols);
    auto offsets = read_1d_int_attr(mu_ds.openAttribute("offsets"));

    int n_e = static_cast<int>(E.size());
    for (int i = 0; i < n_e; ++i) {
      int start = offsets[i];
      int end =
          (i + 1 < static_cast<int>(offsets.size())) ? offsets[i + 1] : cols;
      for (int j = start; j < end; ++j) {
        mu.push_back(mu_data[0 * cols + j]);
        pdf.push_back(mu_data[1 * cols + j]);
        cdf.push_back(mu_data[2 * cols + j]);
      }
      table_offsets.push_back(static_cast<int>(mu.size()));
    }
    return dist_id;
  }
};

struct EnergyPoolBuilder {
  std::vector<int> energy_offsets = {0};
  std::vector<double> energies;
  std::vector<int> table_offsets = {0};
  std::vector<double> E_out, pdf, cdf;
  int n_dists = 0;

  int add(H5::Group &energy_group) {
    int dist_id = n_dists++;

    H5::DataSet e_ds = energy_group.openDataSet("energy");
    auto E = read_1d_double(e_ds);
    energies.insert(energies.end(), E.begin(), E.end());
    energy_offsets.push_back(static_cast<int>(energies.size()));

    H5::DataSet dist_ds = energy_group.openDataSet("distribution");
    int rows, cols;
    auto dist_data = read_2d_double(dist_ds, rows, cols);
    auto offsets = read_1d_int_attr(dist_ds.openAttribute("offsets"));

    int n_e = static_cast<int>(E.size());
    for (int i = 0; i < n_e; ++i) {
      int start = offsets[i];
      int end =
          (i + 1 < static_cast<int>(offsets.size())) ? offsets[i + 1] : cols;
      for (int j = start; j < end; ++j) {
        E_out.push_back(dist_data[0 * cols + j]);
        pdf.push_back(dist_data[1 * cols + j]);
        cdf.push_back(dist_data[2 * cols + j]);
      }
      table_offsets.push_back(static_cast<int>(E_out.size()));
    }
    return dist_id;
  }
};

struct KalbachPoolBuilder {
  std::vector<int> energy_offsets = {0};
  std::vector<double> energies;
  std::vector<int> table_offsets = {0};
  std::vector<double> E_out, pdf, cdf, r, a;
  int n_dists = 0;

  int add(H5::Group &dist_group) {
    int dist_id = n_dists++;

    H5::DataSet e_ds = dist_group.openDataSet("energy");
    auto E = read_1d_double(e_ds);
    energies.insert(energies.end(), E.begin(), E.end());
    energy_offsets.push_back(static_cast<int>(energies.size()));

    H5::DataSet dist_ds = dist_group.openDataSet("distribution");
    int rows, cols;
    auto dist_data = read_2d_double(dist_ds, rows, cols);
    auto offsets = read_1d_int_attr(dist_ds.openAttribute("offsets"));

    int n_e = static_cast<int>(E.size());
    for (int i = 0; i < n_e; ++i) {
      int start = offsets[i];
      int end =
          (i + 1 < static_cast<int>(offsets.size())) ? offsets[i + 1] : cols;
      for (int j = start; j < end; ++j) {
        E_out.push_back(dist_data[0 * cols + j]);
        pdf.push_back(dist_data[1 * cols + j]);
        cdf.push_back(dist_data[2 * cols + j]);
        r.push_back(dist_data[3 * cols + j]);
        a.push_back(dist_data[4 * cols + j]);
      }
      table_offsets.push_back(static_cast<int>(E_out.size()));
    }
    return dist_id;
  }
};

struct FissionYieldPoolBuilder {
  std::vector<FissionYieldDescriptor> entries;
  std::vector<double> energies;
  std::vector<double> nu_bar;

  int add(const std::vector<double> &E, const std::vector<double> &nu) {
    int yield_id = static_cast<int>(entries.size());
    int offset = static_cast<int>(energies.size());
    energies.insert(energies.end(), E.begin(), E.end());
    nu_bar.insert(nu_bar.end(), nu.begin(), nu.end());
    entries.push_back({offset, static_cast<int>(E.size())});
    return yield_id;
  }
};

// ── Yield helper ─────────────────────────────────────────────────────

std::pair<std::vector<double>, std::vector<double>>
yield_to_xy(H5::DataSet &ds, const std::vector<double> &energy_grid) {
  H5::DataSpace space = ds.getSpace();
  int ndims = space.getSimpleExtentNdims();
  if (ndims == 2) {
    int rows, cols;
    auto buf = read_2d_double(ds, rows, cols);
    return {{buf.begin(), buf.begin() + cols},
            {buf.begin() + cols, buf.begin() + 2 * cols}};
  }
  // Constant yield: (1,) → two-point table
  auto buf = read_1d_double(ds);
  return {{energy_grid.front(), energy_grid.back()}, {buf[0], buf[0]}};
}

// ── ZAID discovery ───────────────────────────────────────────────────

// Scan hdf5_dir for *.h5 files. For each, read Z and A attrs to get ZAID.
std::unordered_map<int, std::string>
discover_nuclides(const std::string &hdf5_dir) {
  std::unordered_map<int, std::string> result;
  for (auto &entry : fs::directory_iterator(hdf5_dir)) {
    if (entry.path().extension() != ".h5")
      continue;

    try {
      H5::H5File file(entry.path().string(), H5F_ACC_RDONLY);
      // The top-level group is the nuclide name (e.g. "U235")
      auto names = child_names(file.openGroup("/"));
      for (auto &name : names) {
        H5::Group nuc = file.openGroup("/" + name);
        if (!has_attr(nuc, "Z") || !has_attr(nuc, "A"))
          continue;
        int Z = read_int_attr(nuc, "Z");
        int A = read_int_attr(nuc, "A");
        int zaid = Z * 1000 + A;
        result[zaid] = entry.path().string();
      }
    } catch (const H5::Exception &) {
      // Skip files that can't be opened as HDF5
      continue;
    }
  }
  return result;
}

// ── Temperature selection ────────────────────────────────────────────

std::string select_temperature(H5::Group &nuclide_group, double temperature) {
  H5::Group energy_group = nuclide_group.openGroup("energy");
  auto names = child_names(energy_group);

  std::string best;
  double best_diff = 1e30;
  for (auto &name : names) {
    // Parse "900K" → 900
    int temp_k = 0;
    try {
      temp_k = std::stoi(name);
    } catch (...) {
      continue;
    }
    double diff = std::abs(static_cast<double>(temp_k) - temperature);
    if (diff < best_diff) {
      best_diff = diff;
      best = name;
    }
  }
  if (best.empty())
    throw std::runtime_error("No temperature data found in HDF5 nuclide");
  return best;
}

// ── Reaction key sorting helper ──────────────────────────────────────

// Extract MT number from "reaction_018" → 18
int parse_mt(const std::string &name) {
  auto pos = name.find('_');
  if (pos == std::string::npos)
    return -1;
  return std::stoi(name.substr(pos + 1));
}

} // namespace

// ── Main loader ──────────────────────────────────────────────────────

NuclearDataHost load_nuclear_data_hdf5(const std::string &hdf5_dir,
                                       Material *const *materials,
                                       int n_materials) {
  // 1. Collect unique (ZAID, temperature) pairs
  using ZaidTemp = std::pair<int, double>;
  std::set<ZaidTemp> needed;
  for (int m = 0; m < n_materials; ++m) {
    int n = materials[m]->n_nuclides;
    if (n <= 0) {
      n = 0;
      for (int i = 0; i < MAX_NUCLIDES_PER_MATERIAL; ++i) {
        if (materials[m]->zaids[i] == 0)
          break;
        n++;
      }
    }
    for (int i = 0; i < n; ++i)
      needed.insert({materials[m]->zaids[i], materials[m]->temperature});
  }

  // 2. Discover available HDF5 files
  auto nuclide_files = discover_nuclides(hdf5_dir);

  // 3. Validate
  for (auto &[z, t] : needed) {
    (void)t;
    if (nuclide_files.find(z) == nuclide_files.end())
      throw std::runtime_error("No HDF5 file found for ZAID " +
                               std::to_string(z));
  }

  // 4. Initialize
  NuclearDataHost h;
  AngularPoolBuilder angular_pool;
  EnergyPoolBuilder fission_energy_pool;
  EnergyPoolBuilder delayed_fission_energy_pool;
  KalbachPoolBuilder kalbach_pool;
  FissionYieldPoolBuilder fission_yield_pool;

  // 5. Process each (ZAID, temperature) pair
  for (auto &[zaid, req_temp] : needed) {
    H5::H5File file(nuclide_files[zaid], H5F_ACC_RDONLY);

    // Find the nuclide group name
    H5::Group root = file.openGroup("/");
    std::string nuc_name;
    for (auto &name : child_names(root)) {
      H5::Group g = file.openGroup("/" + name);
      if (has_attr(g, "Z") && has_attr(g, "A")) {
        nuc_name = name;
        break;
      }
    }
    H5::Group nuc = file.openGroup("/" + nuc_name);

    double awr = read_double_attr(nuc, "atomic_weight_ratio");
    std::string temp_str = select_temperature(nuc, req_temp);

    // Read energy grid
    H5::DataSet e_grid_ds = nuc.openDataSet("energy/" + temp_str);
    auto E_grid = read_1d_double(e_grid_ds);
    int grid_offset = static_cast<int>(h.energy_grids.size());
    int grid_length = static_cast<int>(E_grid.size());
    h.energy_grids.insert(h.energy_grids.end(), E_grid.begin(), E_grid.end());

    int rxn_offset = static_cast<int>(h.reactions.size());

    // Collect and sort reaction keys
    H5::Group rxn_group = nuc.openGroup("reactions");
    auto rxn_names = child_names(rxn_group);
    std::sort(rxn_names.begin(), rxn_names.end(),
              [](const std::string &a, const std::string &b) {
                return parse_mt(a) < parse_mt(b);
              });

    // Absorption accumulator
    std::vector<double> absorption_xs(grid_length, 0.0);
    bool has_absorption = false;

    for (auto &rk : rxn_names) {
      H5::Group r = rxn_group.openGroup(rk);
      int mt = read_int_attr(r, "mt");
      int redundant =
          has_attr(r, "redundant") ? read_int_attr(r, "redundant") : 0;

      if (redundant)

        continue;

      // Read XS helper lambda
      auto read_xs = [&](int &thresh) -> std::vector<double> {
        H5::Group temp_g = r.openGroup(temp_str);
        H5::DataSet xs_ds = temp_g.openDataSet("xs");
        auto xs = read_1d_double(xs_ds);
        thresh = has_attr(xs_ds, "threshold_idx")
                     ? read_int_attr(xs_ds, "threshold_idx")
                     : 0;
        return xs;
      };

      if ((mt >= 102 && mt <= 117) || (mt >= 600 && mt <= 849)) {
        // Absorption — accumulate
        int thresh;
        auto xs = read_xs(thresh);
        for (int j = 0; j < static_cast<int>(xs.size()); ++j)
          absorption_xs[thresh + j] += xs[j];
        has_absorption = true;

      } else if (mt == 2) {
        // Elastic
        int thresh;
        auto xs = read_xs(thresh);
        int xs_off = static_cast<int>(h.xs_pool.size());
        h.xs_pool.insert(h.xs_pool.end(), xs.begin(), xs.end());

        int dist_id = -1;
        H5::Group d0 = r.openGroup("product_0/distribution_0");
        if (has_child(d0, "angle")) {
          H5::Group angle = d0.openGroup("angle");
          dist_id = angular_pool.add(angle);
        }

        ReactionDescriptor rd{};
        rd.type = RxnType::ELASTIC;
        rd.xs_offset = xs_off;
        rd.xs_length = static_cast<int>(xs.size());
        rd.threshold_idx = thresh;
        rd.dist_id = dist_id;
        rd.Q_value = 0.0;
        rd.multiplicity = 1;
        rd.yield_id = -1;
        rd.delayed_id = -1;
        h.reactions.push_back(rd);

      } else if (mt == 18) {
        // Fission
        int thresh;
        auto xs = read_xs(thresh);
        int xs_off = static_cast<int>(h.xs_pool.size());
        h.xs_pool.insert(h.xs_pool.end(), xs.begin(), xs.end());

        // Prompt neutron energy distribution
        H5::Group p0 = r.openGroup("product_0");
        H5::Group d0_energy = p0.openGroup("distribution_0/energy");
        int dist_id = fission_energy_pool.add(d0_energy);

        // Prompt nu-bar
        H5::DataSet yield_ds = p0.openDataSet("yield");
        auto [nu_E, nu_prompt] = yield_to_xy(yield_ds, E_grid);
        int yield_id = fission_yield_pool.add(nu_E, nu_prompt);

        // Delayed neutron groups
        int group_offset = static_cast<int>(h.delayed_groups.size());
        int n_groups = 0;
        auto product_names = child_names(r);
        std::sort(product_names.begin(), product_names.end());
        for (auto &pk : product_names) {
          if (pk == "product_0" || pk.substr(0, 8) != "product_")
            continue;
          H5::Group p = r.openGroup(pk);
          if (!has_attr(p, "particle") || !has_attr(p, "emission_mode"))
            continue;
          std::string particle = read_string_attr(p, "particle");
          std::string emission = read_string_attr(p, "emission_mode");
          if (particle != "neutron" || emission != "delayed")
            continue;
          if (!has_child(p, "yield"))
            continue;
          if (!has_child(p, "distribution_0"))
            continue;
          H5::Group dd0 = p.openGroup("distribution_0");
          if (!has_child(dd0, "energy"))
            continue;

          H5::DataSet d_yield_ds = p.openDataSet("yield");
          auto [dE, dnu] = yield_to_xy(d_yield_ds, E_grid);
          int delayed_yield_id = fission_yield_pool.add(dE, dnu);

          H5::Group d_energy = dd0.openGroup("energy");
          int delayed_dist_id = delayed_fission_energy_pool.add(d_energy);

          h.delayed_groups.push_back({delayed_yield_id, delayed_dist_id});
          n_groups++;
        }

        int delayed_id = -1;
        if (n_groups > 0) {
          delayed_id = static_cast<int>(h.delayed_fission_descs.size());
          h.delayed_fission_descs.push_back({group_offset, n_groups});
        }
        double Q_value =
            has_attr(r, "Q_value") ? read_double_attr(r, "Q_value") : 0.0;

        ReactionDescriptor rd{};
        rd.type = RxnType::FISSION;
        rd.xs_offset = xs_off;
        rd.xs_length = static_cast<int>(xs.size());
        rd.threshold_idx = thresh;
        rd.dist_id = dist_id;
        rd.Q_value = Q_value;
        rd.multiplicity = -1;
        rd.yield_id = yield_id;
        rd.delayed_id = delayed_id;
        h.reactions.push_back(rd);

      } else if (mt >= 51 && mt <= 90) {
        // Discrete inelastic
        int thresh;
        auto xs = read_xs(thresh);
        int xs_off = static_cast<int>(h.xs_pool.size());
        h.xs_pool.insert(h.xs_pool.end(), xs.begin(), xs.end());

        int dist_id = -1;
        H5::Group p0 = r.openGroup("product_0");
        H5::Group d0 = p0.openGroup("distribution_0");
        if (has_child(d0, "angle")) {
          H5::Group angle = d0.openGroup("angle");
          dist_id = angular_pool.add(angle);
        }

        double Q_value = read_double_attr(r, "Q_value");

        ReactionDescriptor rd{};
        rd.type = RxnType::DISCRETE_INELASTIC;
        rd.xs_offset = xs_off;
        rd.xs_length = static_cast<int>(xs.size());
        rd.threshold_idx = thresh;
        rd.dist_id = dist_id;
        rd.Q_value = Q_value;
        rd.multiplicity = 1;
        rd.yield_id = -1;
        rd.delayed_id = -1;
        h.reactions.push_back(rd);

      } else if (mt == 91) {
        // Continuum inelastic (Kalbach-Mann)
        int thresh;
        auto xs = read_xs(thresh);
        int xs_off = static_cast<int>(h.xs_pool.size());
        h.xs_pool.insert(h.xs_pool.end(), xs.begin(), xs.end());

        H5::Group d0 = r.openGroup("product_0/distribution_0");
        int dist_id = kalbach_pool.add(d0);
        double Q_value = read_double_attr(r, "Q_value");

        ReactionDescriptor rd{};
        rd.type = RxnType::CONTINUUM_INELASTIC;
        rd.xs_offset = xs_off;
        rd.xs_length = static_cast<int>(xs.size());
        rd.threshold_idx = thresh;
        rd.dist_id = dist_id;
        rd.Q_value = Q_value;
        rd.multiplicity = 1;
        rd.yield_id = -1;
        rd.delayed_id = -1;
        h.reactions.push_back(rd);

      } else if (mt == 16 || mt == 17) {
        // (n,2n) or (n,3n)
        int thresh;
        auto xs = read_xs(thresh);
        int xs_off = static_cast<int>(h.xs_pool.size());
        h.xs_pool.insert(h.xs_pool.end(), xs.begin(), xs.end());

        H5::Group d0 = r.openGroup("product_0/distribution_0");
        int dist_id = kalbach_pool.add(d0);
        double Q_value = read_double_attr(r, "Q_value");

        ReactionDescriptor rd{};
        rd.type = (mt == 16) ? RxnType::N2N : RxnType::N3N;
        rd.xs_offset = xs_off;
        rd.xs_length = static_cast<int>(xs.size());
        rd.threshold_idx = thresh;
        rd.dist_id = dist_id;
        rd.Q_value = Q_value;
        rd.multiplicity = (mt == 16) ? 2 : 3;
        rd.yield_id = -1;
        rd.delayed_id = -1;
        h.reactions.push_back(rd);
      }
    }

    // Add summed absorption channel
    if (has_absorption) {
      int xs_off = static_cast<int>(h.xs_pool.size());
      h.xs_pool.insert(h.xs_pool.end(), absorption_xs.begin(),
                       absorption_xs.end());

      ReactionDescriptor rd{};
      rd.type = RxnType::ABSORPTION;
      rd.xs_offset = xs_off;
      rd.xs_length = grid_length;
      rd.threshold_idx = 0;
      rd.dist_id = -1;
      rd.Q_value = 0.0;
      rd.multiplicity = 0;
      rd.yield_id = -1;
      rd.delayed_id = -1;
      h.reactions.push_back(rd);
    }

    int n_reactions = static_cast<int>(h.reactions.size()) - rxn_offset;

    int kappa_id = -1;
    if (has_child(nuc, "fission_energy_release")) {
      H5::Group fer = nuc.openGroup("fission_energy_release");
      if (has_child(fer, "q_recoverable")) {
        H5::DataSet q_ds = fer.openDataSet("q_recoverable");
        auto q = read_1d_double(q_ds); 
        if (q.size() != 3) {
          throw std::runtime_error{"Expected 3 polynomial coefficients in q_recoverable for " + nuc_name + " recieved " + std::to_string(q.size())}; 
        }

        KappaDescriptor kd{};
        kd.coeffs[0] = q[0];
        kd.coeffs[1] = q[1];
        kd.coeffs[2] = q[2];

        kappa_id = static_cast<int>(h.kappa_descs.size());
        h.kappa_descs.push_back(kd);
      }
    }

    NuclideDescriptor nd{};
    nd.grid_offset = grid_offset;
    nd.grid_length = grid_length;
    nd.A = awr;
    nd.rxn_offset = rxn_offset;
    nd.n_reactions = n_reactions;
    nd.zaid = zaid;
    nd.temperature = req_temp;
    nd.kappa_id = kappa_id;
    h.nuclides.push_back(nd);

    std::cout << "Loaded " << std::left << std::setw(9) << nuc_name
              << "  ZAID:" << std::right << std::setw(6) << zaid
              << "  T:" << std::setw(5) << temp_str
              << "  reactions:" << std::setw(3) << n_reactions
              << "  grid:" << std::setw(7) << grid_length << "\n";
  }
  std::cout << "\n";
  // 6. Move pool data into NuclearDataHost
  h.ang_energy_offsets = std::move(angular_pool.energy_offsets);
  h.ang_energies = std::move(angular_pool.energies);
  h.ang_table_offsets = std::move(angular_pool.table_offsets);
  h.ang_mu = std::move(angular_pool.mu);
  h.ang_pdf = std::move(angular_pool.pdf);
  h.ang_cdf = std::move(angular_pool.cdf);
  h.fe_energy_offsets = std::move(fission_energy_pool.energy_offsets);
  h.fe_energies = std::move(fission_energy_pool.energies);
  h.fe_table_offsets = std::move(fission_energy_pool.table_offsets);
  h.fe_E_out = std::move(fission_energy_pool.E_out);
  h.fe_pdf = std::move(fission_energy_pool.pdf);
  h.fe_cdf = std::move(fission_energy_pool.cdf);

  h.dfe_energy_offsets = std::move(delayed_fission_energy_pool.energy_offsets);
  h.dfe_energies = std::move(delayed_fission_energy_pool.energies);
  h.dfe_table_offsets = std::move(delayed_fission_energy_pool.table_offsets);
  h.dfe_E_out = std::move(delayed_fission_energy_pool.E_out);
  h.dfe_pdf = std::move(delayed_fission_energy_pool.pdf);
  h.dfe_cdf = std::move(delayed_fission_energy_pool.cdf);

  h.km_energy_offsets = std::move(kalbach_pool.energy_offsets);
  h.km_energies = std::move(kalbach_pool.energies);
  h.km_table_offsets = std::move(kalbach_pool.table_offsets);
  h.km_E_out = std::move(kalbach_pool.E_out);
  h.km_pdf = std::move(kalbach_pool.pdf);
  h.km_cdf = std::move(kalbach_pool.cdf);
  h.km_r = std::move(kalbach_pool.r);
  h.km_a = std::move(kalbach_pool.a);

  h.fission_yield_descs = std::move(fission_yield_pool.entries);
  h.fy_energy = std::move(fission_yield_pool.energies);
  h.fy_nu_bar = std::move(fission_yield_pool.nu_bar);

  return h;
}
