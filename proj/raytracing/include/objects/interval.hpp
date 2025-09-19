#ifndef __INTERVAL_HPP__
#define __INTERVAL_HPP__

#include <constants.hpp>

class interval {
public:
  double min, max;

  interval(): min(INF), max(-INF) {}
  interval(double _min, double _max): min(_min), max(_max) {}

  double size() const;
  bool contains(double x) const;
  bool surrounds(double x) const;
  double clamp(double x) const;

  static const interval empty;
  static const interval universe;
};

#endif