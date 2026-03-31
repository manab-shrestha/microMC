#pragma once

#include "rng.h"

// Sample an isotropic direction uniformly on the unit sphere
void sample_isodir(double &Omega_x, double &Omega_y, double &Omega_z,
                   RNG &rng);

// Rotate direction vector by scattering cosine mu with random azimuthal angle
void rotate_dir(double &Omega_x, double &Omega_y, double &Omega_z, double mu,
                RNG &rng);
