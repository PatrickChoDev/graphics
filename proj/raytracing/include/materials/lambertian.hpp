#ifndef __LAMBERTIAN_HPP__
#define __LAMBERTIAN_HPP__

#include <materials/base.hpp>

class lambertian : public material {
public:
  color albedo;

  lambertian(const color& a) : albedo(a) {}

  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

#endif