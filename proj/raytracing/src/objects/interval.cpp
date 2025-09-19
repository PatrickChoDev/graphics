#include <objects/interval.hpp>

// interval method definitions
double interval::size() const {
  return max - min;
}

bool interval::contains(double x) const {
  return min <= x && x <= max;
}

bool interval::surrounds(double x) const {
  return min < x && x < max;
}

double interval::clamp(double x) const {
  if (x < min) {
    return min;
  }
  if (x > max) {
    return max;
  }
  return x;
}

const interval interval::empty = interval(INF, -INF);
const interval interval::universe = interval(-INF, INF);