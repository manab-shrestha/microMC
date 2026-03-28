#ifndef RNG_H
#define RNG_H

#include <random>

using RNG = std::mt19937_64;

// Sample a uniform random number in [0, 1)
inline double uniform(RNG& rng) {
    return std::uniform_real_distribution<double>{0.0, 1.0}(rng);
}

#endif
