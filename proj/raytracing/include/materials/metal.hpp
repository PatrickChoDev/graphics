#ifndef __METAL_HPP__
#define __METAL_HPP__

#include <materials/base.hpp>

class metal : public material {
public:
  color albedo;
  double fuzz;

  metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

#endif