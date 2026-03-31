#pragma once

#include <random>

using RNG = std::mt19937_64;

inline double uniform(RNG &rng) {
  return (rng() >> 11) * (1.0 / 9007199254740992.0); // 1 / 2^53
}
