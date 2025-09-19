#ifndef __RANDOMIZER_HPP__
#define __RANDOMIZER_HPP__

#include <random>

inline double random_double() {
  // Thread-local PRNG to allow safe parallel rendering without contention.
  // Each thread gets its own generator seeded from std::random_device.
  thread_local std::mt19937 generator(std::random_device{}());
  thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
  return distribution(generator);
}

inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

#endif