#include "nuclear_data.h"

#include <fstream>
#include <stdexcept>

const char *rxn_type_name(RxnType type) {
  switch (type) {
  case RxnType::ELASTIC:
    return "elastic";
  case RxnType::DISCRETE_INELASTIC:
    return "disc_inel";
  case RxnType::CONTINUUM_INELASTIC:
    return "cont_inel";
  case RxnType::FISSION:
    return "fission";
  case RxnType::ABSORPTION:
    return "absorption";
  case RxnType::N2N:
    return "(n,2n)";
  case RxnType::N3N:
    return "(n,3n)";
  }
  return "unknown";
}

namespace {

constexpr int FILE_MAGIC = 0x4D434442; // "MCDB"
constexpr int FILE_VERSION = 1;

void read_i32(std::ifstream &f, int &v) {
  f.read(reinterpret_cast<char *>(&v), sizeof(int));
}

void read_f64(std::ifstream &f, double &v) {
  f.read(reinterpret_cast<char *>(&v), sizeof(double));
}

void read_i32_vec(std::ifstream &f, std::vector<int> &v, int n) {
  v.resize(n);
  f.read(reinterpret_cast<char *>(v.data()),
         static_cast<std::streamsize>(n * sizeof(int)));
}

void read_f64_vec(std::ifstream &f, std::vector<double> &v, int n) {
  v.resize(n);
  f.read(reinterpret_cast<char *>(v.data()),
         static_cast<std::streamsize>(n * sizeof(double)));
}

} // namespace

NuclearData NuclearDataHost::view() const {
  NuclearData d{};

  d.nuclides = nuclides.data();
  d.n_nuclides = static_cast<int>(nuclides.size());
  d.reactions = reactions.data();
  d.n_reactions = static_cast<int>(reactions.size());
  d.energy_grids = energy_grids.data();
  d.n_energy_points = static_cast<int>(energy_grids.size());
  d.xs_pool = xs_pool.data();
  d.n_xs_points = static_cast<int>(xs_pool.size());

  // Angular
  d.angular.energy_offsets = ang_energy_offsets.data();
  d.angular.energies = ang_energies.data();
  d.angular.table_offsets = ang_table_offsets.data();
  d.angular.mu = ang_mu.data();
  d.angular.pdf = ang_pdf.data();
  d.angular.cdf = ang_cdf.data();
  d.angular.n_dists = static_cast<int>(ang_energy_offsets.size()) - 1;
  d.angular.total_energy_points = static_cast<int>(ang_energies.size());
  d.angular.total_mu_points = static_cast<int>(ang_mu.size());

  // Fission energy
  d.fission_energy.energy_offsets = fe_energy_offsets.data();
  d.fission_energy.energies = fe_energies.data();
  d.fission_energy.table_offsets = fe_table_offsets.data();
  d.fission_energy.E_out = fe_E_out.data();
  d.fission_energy.pdf = fe_pdf.data();
  d.fission_energy.cdf = fe_cdf.data();
  d.fission_energy.n_dists = static_cast<int>(fe_energy_offsets.size()) - 1;
  d.fission_energy.total_energy_points = static_cast<int>(fe_energies.size());
  d.fission_energy.total_out_points = static_cast<int>(fe_E_out.size());

  // Kalbach
  d.kalbach.energy_offsets = km_energy_offsets.data();
  d.kalbach.energies = km_energies.data();
  d.kalbach.table_offsets = km_table_offsets.data();
  d.kalbach.E_out = km_E_out.data();
  d.kalbach.pdf = km_pdf.data();
  d.kalbach.cdf = km_cdf.data();
  d.kalbach.r = km_r.data();
  d.kalbach.a = km_a.data();
  d.kalbach.n_dists = static_cast<int>(km_energy_offsets.size()) - 1;
  d.kalbach.total_energy_points = static_cast<int>(km_energies.size());
  d.kalbach.total_out_points = static_cast<int>(km_E_out.size());

  // Fission yields
  d.fission_yields.descriptors = fission_yield_descs.data();
  d.fission_yields.energy = fy_energy.data();
  d.fission_yields.nu_bar = fy_nu_bar.data();
  d.fission_yields.n_yields = static_cast<int>(fission_yield_descs.size());
  d.fission_yields.total_points = static_cast<int>(fy_energy.size());

  return d;
}

NuclearDataHost load_nuclear_data(const std::string &path) {
  std::ifstream f(path, std::ios::binary);
  if (!f)
    throw std::runtime_error("Failed to open " + path);

  NuclearDataHost h;

  // ── Header ──────────────────────────────────────────────────────
  int n_nuclides, n_reactions, n_energy_points, n_xs_points;
  int first_word;
  read_i32(f, first_word);

  if (first_word == FILE_MAGIC) {
    int version;
    read_i32(f, version);
    if (version != FILE_VERSION)
      throw std::runtime_error("Unsupported nuclear data version in " + path);
    read_i32(f, n_nuclides);
    read_i32(f, n_reactions);
    read_i32(f, n_energy_points);
    read_i32(f, n_xs_points);
  } else {
    // Backward-compatible path for older files without magic/version.
    n_nuclides = first_word;
    read_i32(f, n_reactions);
    read_i32(f, n_energy_points);
    read_i32(f, n_xs_points);
  }

  int ang_n_dists, ang_n_energies, ang_n_mu;
  read_i32(f, ang_n_dists);
  read_i32(f, ang_n_energies);
  read_i32(f, ang_n_mu);

  int fe_n_dists, fe_n_energies, fe_n_out;
  read_i32(f, fe_n_dists);
  read_i32(f, fe_n_energies);
  read_i32(f, fe_n_out);

  int km_n_dists, km_n_energies, km_n_out;
  read_i32(f, km_n_dists);
  read_i32(f, km_n_energies);
  read_i32(f, km_n_out);

  int fy_n_yields, fy_n_points;
  read_i32(f, fy_n_yields);
  read_i32(f, fy_n_points);

  // ── Nuclide descriptors ─────────────────────────────────────────
  h.nuclides.resize(n_nuclides);
  for (int i = 0; i < n_nuclides; ++i) {
    auto &nd = h.nuclides[i];
    read_i32(f, nd.grid_offset);
    read_i32(f, nd.grid_length);
    read_f64(f, nd.A);
    read_i32(f, nd.rxn_offset);
    read_i32(f, nd.n_reactions);
    read_i32(f, nd.zaid);
  }

  // ── Reaction descriptors ────────────────────────────────────────
  h.reactions.resize(n_reactions);
  for (int i = 0; i < n_reactions; ++i) {
    auto &rd = h.reactions[i];
    int raw_type;
    read_i32(f, raw_type);
    rd.type = static_cast<RxnType>(raw_type);
    read_i32(f, rd.xs_offset);
    read_i32(f, rd.xs_length);
    read_i32(f, rd.threshold_idx);
    read_i32(f, rd.dist_id);
    read_f64(f, rd.Q_value);
    read_i32(f, rd.multiplicity);
    read_i32(f, rd.yield_id);
  }

  // ── Fission yield descriptors ───────────────────────────────────
  h.fission_yield_descs.resize(fy_n_yields);
  for (int i = 0; i < fy_n_yields; ++i) {
    read_i32(f, h.fission_yield_descs[i].offset);
    read_i32(f, h.fission_yield_descs[i].n_points);
  }

  // ── Double pools ────────────────────────────────────────────────
  read_f64_vec(f, h.energy_grids, n_energy_points);
  read_f64_vec(f, h.xs_pool, n_xs_points);

  // Angular
  read_f64_vec(f, h.ang_energies, ang_n_energies);
  read_f64_vec(f, h.ang_mu, ang_n_mu);
  read_f64_vec(f, h.ang_pdf, ang_n_mu);
  read_f64_vec(f, h.ang_cdf, ang_n_mu);

  // Fission energy
  read_f64_vec(f, h.fe_energies, fe_n_energies);
  read_f64_vec(f, h.fe_E_out, fe_n_out);
  read_f64_vec(f, h.fe_pdf, fe_n_out);
  read_f64_vec(f, h.fe_cdf, fe_n_out);

  // Kalbach
  read_f64_vec(f, h.km_energies, km_n_energies);
  read_f64_vec(f, h.km_E_out, km_n_out);
  read_f64_vec(f, h.km_pdf, km_n_out);
  read_f64_vec(f, h.km_cdf, km_n_out);
  read_f64_vec(f, h.km_r, km_n_out);
  read_f64_vec(f, h.km_a, km_n_out);

  // Fission yield
  read_f64_vec(f, h.fy_energy, fy_n_points);
  read_f64_vec(f, h.fy_nu_bar, fy_n_points);

  // ── Int pools ───────────────────────────────────────────────────
  read_i32_vec(f, h.ang_energy_offsets, ang_n_dists + 1);
  read_i32_vec(f, h.ang_table_offsets, ang_n_energies + 1);

  read_i32_vec(f, h.fe_energy_offsets, fe_n_dists + 1);
  read_i32_vec(f, h.fe_table_offsets, fe_n_energies + 1);

  read_i32_vec(f, h.km_energy_offsets, km_n_dists + 1);
  read_i32_vec(f, h.km_table_offsets, km_n_energies + 1);

  if (!f)
    throw std::runtime_error("Unexpected end of file reading " + path);

  return h;
}
