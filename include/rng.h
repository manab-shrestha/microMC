#pragma once

#include <cstdint>
#include <limits>

class RNG {
public:
  using result_type = uint64_t;

  explicit RNG(uint64_t seed = 0x9e3779b97f4a7c15ULL) {
    uint64_t x = seed;
    for (int i = 0; i < 4; ++i)
      s_[i] = splitmix64(x);
    if ((s_[0] | s_[1] | s_[2] | s_[3]) == 0)
      s_[0] = 0x9e3779b97f4a7c15ULL;
  }

  static constexpr result_type min() { return 0; }

  static constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }

  result_type operator()() {
    const uint64_t result = rotl(s_[1] * 5ULL, 7) * 9ULL;
    const uint64_t t = s_[1] << 17;

    s_[2] ^= s_[0];
    s_[3] ^= s_[1];
    s_[1] ^= s_[2];
    s_[0] ^= s_[3];

    s_[2] ^= t;
    s_[3] = rotl(s_[3], 45);

    return result;
  }

private:
  uint64_t s_[4]{};

  static uint64_t rotl(uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }

  static uint64_t splitmix64(uint64_t &x) {
    uint64_t z = (x += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
  }
};

inline double uniform(RNG &rng) {
  return (rng() >> 11) * (1.0 / 9007199254740992.0);
}
