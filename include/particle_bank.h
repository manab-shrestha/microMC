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

  void reserve(int n) {
    if (n < 0)
      throw std::runtime_error("ParticleBankHost::reserve negative n");
    capacity = n;

    x.reserve(n);
    y.reserve(n);
    z.reserve(n);
    E.reserve(n);
    Omega_x.reserve(n);
    Omega_y.reserve(n);
    Omega_z.reserve(n);
    w.reserve(n);
    macro_xs_t.reserve(n);
    alive.reserve(n);
    rxn_nuclide_idx.reserve(n);
    rxn_rxn_idx.reserve(n);
    rxn_type.reserve(n);
    rng.reserve(n);
  };

  void resize(int n) {
    if (n < 0)
      throw std::runtime_error("ParticleBankHost::resize negative n");
    if (n > capacity)
      reserve(n);

    size = n;

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

  void clear() {
    size = 0;
    resize(0);
  };

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
