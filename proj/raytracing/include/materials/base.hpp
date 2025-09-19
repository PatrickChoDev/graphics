#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <objects/color.hpp>
#include <objects/hit_record.hpp>
#include <objects/ray.hpp>
#include <objects/vec3.hpp>

class material {
public:
  virtual ~material() = default;
  
  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

#endif