#pragma once

#include "rng.h"
#include <stdexcept>
#include <vector>

struct ParticleBankView {
  double *x;
  double *y;
  double *z;
  double *E;
  double *Omega_x;
  double *Omega_y;
  double *Omega_z;
  double *w;
  double *macro_xs_t;
  int *alive;
  int *rxn_nuclide_idx;
  int *rxn_rxn_idx;
  int *rxn_type;
  RNG *rng;
  int size;
  int capacity;
};

struct ParticleBankConstView {
  const double *x;
  const double *y;
  const double *z;
  const double *E;
  const double *Omega_x;
  const double *Omega_y;
  const double *Omega_z;
  const double *w;
  const double *macro_xs_t;
  const int *alive;
  const int *rxn_nuclide_idx;
  const int *rxn_rxn_idx;
  const int *rxn_type;
  const RNG *rng;
  const int size;
  const int capacity;
};

struct ParticleBankHost {
  std::vector<double> x, y, z, E, Omega_x, Omega_y, Omega_z, w, macro_xs_t;
  std::vector<int> alive, rxn_nuclide_idx, rxn_rxn_idx, rxn_type;
  std::vector<RNG> rng;

  int size = 0;
  int capacity = 0;

  void set_capacity(int n) {
    if (n < 0)
      throw std::runtime_error("ParticleBankHost::set_capacity negative n");
    if (n < size)
      throw std::runtime_error(
          "ParticleBankHost::set_capacity n smaller than current size");

    capacity = n;

    x.resize(n);
    y.resize(n);
    z.resize(n);
    E.resize(n);
    Omega_x.resize(n);
    Omega_y.resize(n);
    Omega_z.resize(n);
    w.resize(n);
    macro_xs_t.resize(n);
    alive.resize(n);
    rxn_nuclide_idx.resize(n);
    rxn_rxn_idx.resize(n);
    rxn_type.resize(n);
    rng.resize(n);
  };

  void set_size(int n) {
    if (n < 0)
      throw std::runtime_error("ParticleBankHost::set_size negative n");
    if (n > capacity)
      throw std::runtime_error(
          "ParticleBankHost::set_size n exceeds capacity");
    size = n;
  };

  void clear_active() { size = 0; };

  ParticleBankView view() {
    return ParticleBankView{
        x.data(),
        y.data(),
        z.data(),
        E.data(),
        Omega_x.data(),
        Omega_y.data(),
        Omega_z.data(),
        w.data(),
        macro_xs_t.data(),
        alive.data(),
        rxn_nuclide_idx.data(),
        rxn_rxn_idx.data(),
        rxn_type.data(),
        rng.data(),
        size,
        capacity,
    };
  };

  ParticleBankConstView view() const {
    return ParticleBankConstView{
        x.data(),
        y.data(),
        z.data(),
        E.data(),
        Omega_x.data(),
        Omega_y.data(),
        Omega_z.data(),
        w.data(),
        macro_xs_t.data(),
        alive.data(),
        rxn_nuclide_idx.data(),
        rxn_rxn_idx.data(),
        rxn_type.data(),
        rng.data(),
        size,
        capacity,
    };
  };
};

inline void copy_particle_fields(ParticleBankView bank, int dst, int src) {
  if (dst < 0 || src < 0 || dst >= bank.capacity || src >= bank.capacity)
    throw std::runtime_error("copy_particle_fields index out of bounds");

  bank.x[dst] = bank.x[src];
  bank.y[dst] = bank.y[src];
  bank.z[dst] = bank.z[src];
  bank.E[dst] = bank.E[src];
  bank.Omega_x[dst] = bank.Omega_x[src];
  bank.Omega_y[dst] = bank.Omega_y[src];
  bank.Omega_z[dst] = bank.Omega_z[src];
  bank.w[dst] = bank.w[src];
  bank.macro_xs_t[dst] = bank.macro_xs_t[src];
  bank.alive[dst] = bank.alive[src];
  bank.rxn_nuclide_idx[dst] = bank.rxn_nuclide_idx[src];
  bank.rxn_rxn_idx[dst] = bank.rxn_rxn_idx[src];
  bank.rxn_type[dst] = bank.rxn_type[src];
  bank.rng[dst] = bank.rng[src];
}

inline void init_particle_defaults(ParticleBankView bank, int i) {
  if (i < 0 || i >= bank.capacity)
    throw std::runtime_error("init_particle_defaults index out of bounds");

  bank.x[i] = 0.0;
  bank.y[i] = 0.0;
  bank.z[i] = 0.0;
  bank.E[i] = 0.0;
  bank.Omega_x[i] = 0.0;
  bank.Omega_y[i] = 0.0;
  bank.Omega_z[i] = 0.0;
  bank.w[i] = 1.0;
  bank.macro_xs_t[i] = 0.0;
  bank.alive[i] = 1;
  bank.rxn_nuclide_idx[i] = 0;
  bank.rxn_rxn_idx[i] = 0;
  bank.rxn_type[i] = 0;
  bank.rng[i] = RNG{};
}
