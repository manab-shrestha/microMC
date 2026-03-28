#ifndef NUCLEAR_DATA_H
#define NUCLEAR_DATA_H

#include <cstdint>
#include <string>
#include <vector>

// ── Reaction type enum (matches Python extractor) ──────────────────
enum class RxnType : int {
  ELASTIC = 0,
  DISCRETE_INELASTIC = 1,
  CONTINUUM_INELASTIC = 2,
  FISSION = 3,
  ABSORPTION = 4,
  N2N = 5,
  N3N = 6,
};

// ── POD descriptors (device-compatible, no std:: types) ────────────
struct NuclideDescriptor {
  int grid_offset;
  int grid_length;
  double A;
  int rxn_offset;
  int n_reactions;
  int zaid;
};

struct ReactionDescriptor {
  int type;      // RxnType as int
  int xs_offset; // into xs_pool
  int xs_length;
  int threshold_idx; // index into nuclide energy grid where xs[0] begins
  int dist_id; // into angular / fission_energy / kalbach pool (-1 if none)
  double Q_value;
  int multiplicity; // 1 for most; 2 for (n,2n); 3 for (n,3n); -1 for fission
                    // (energy-dep nu)
  int yield_id;     // into fission_yields (-1 if not fission)
};

struct FissionYieldDescriptor {
  int offset; // into fission yield energy/nu_bar arrays
  int n_points;
};

// ── Distribution pools (POD, flat arrays) ──────────────────────────

struct AngularDistPool {
  const int *energy_offsets; // [n_dists + 1]
  const double *energies;    // [total_energy_points]
  const int *table_offsets;  // [total_energy_points + 1]
  const double *mu;          // [total_mu_points]
  const double *pdf;         // [total_mu_points]
  const double *cdf;         // [total_mu_points]
  int n_dists;
  int total_energy_points;
  int total_mu_points;
};

struct EnergyDistPool {
  const int *energy_offsets; // [n_dists + 1]
  const double *energies;    // [total_energy_points]
  const int *table_offsets;  // [total_energy_points + 1]
  const double *E_out;       // [total_out_points]
  const double *pdf;         // [total_out_points]
  const double *cdf;         // [total_out_points]
  int n_dists;
  int total_energy_points;
  int total_out_points;
};

struct KalbachMannDistPool {
  const int *energy_offsets; // [n_dists + 1]
  const double *energies;    // [total_energy_points]
  const int *table_offsets;  // [total_energy_points + 1]
  const double *E_out;       // [total_out_points]
  const double *pdf;         // [total_out_points]
  const double *cdf;         // [total_out_points]
  const double *r;           // [total_out_points]
  const double *a;           // [total_out_points]
  int n_dists;
  int total_energy_points;
  int total_out_points;
};

struct FissionYieldPool {
  const FissionYieldDescriptor *descriptors; // [n_yields]
  const double *energy;                      // [total_points]
  const double *nu_bar;                      // [total_points]
  int n_yields;
  int total_points;
};

// ── Top-level nuclear data (all pools) ─────────────────────────────

struct NuclearData {
  const NuclideDescriptor *nuclides;
  int n_nuclides;
  const ReactionDescriptor *reactions;
  int n_reactions;

  const double *energy_grids;
  int n_energy_points;
  const double *xs_pool;
  int n_xs_points;

  AngularDistPool angular;
  EnergyDistPool fission_energy;
  KalbachMannDistPool kalbach;
  FissionYieldPool fission_yields;
};

// ── Host-side owning container ─────────────────────────────────────
// Holds std::vectors that own the memory. The NuclearData struct above
// points into these vectors.

struct NuclearDataHost {
  std::vector<NuclideDescriptor> nuclides;
  std::vector<ReactionDescriptor> reactions;
  std::vector<FissionYieldDescriptor> fission_yield_descs;

  std::vector<double> energy_grids;
  std::vector<double> xs_pool;

  // Angular pool
  std::vector<int> ang_energy_offsets;
  std::vector<double> ang_energies;
  std::vector<int> ang_table_offsets;
  std::vector<double> ang_mu;
  std::vector<double> ang_pdf;
  std::vector<double> ang_cdf;

  // Fission energy pool
  std::vector<int> fe_energy_offsets;
  std::vector<double> fe_energies;
  std::vector<int> fe_table_offsets;
  std::vector<double> fe_E_out;
  std::vector<double> fe_pdf;
  std::vector<double> fe_cdf;

  // Kalbach pool
  std::vector<int> km_energy_offsets;
  std::vector<double> km_energies;
  std::vector<int> km_table_offsets;
  std::vector<double> km_E_out;
  std::vector<double> km_pdf;
  std::vector<double> km_cdf;
  std::vector<double> km_r;
  std::vector<double> km_a;

  // Fission yield data
  std::vector<double> fy_energy;
  std::vector<double> fy_nu_bar;

  // Build a NuclearData view pointing into this container's data
  NuclearData view() const;
};

// Load from binary file produced by extract_hdf5.py
NuclearDataHost load_nuclear_data(const std::string &path);

// Human-readable name for a RxnType value
const char *rxn_type_name(int type);

#endif
