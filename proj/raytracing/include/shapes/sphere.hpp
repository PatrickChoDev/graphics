#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include <algorithm>
#include <memory>

#include <objects/color.hpp>
#include <objects/hittable.hpp>

#include <materials/base.hpp>

class sphere: public hittable {
public:
  point3 center;
  double radius;
  std::shared_ptr<material> mat;

  sphere(point3 _center, double _radius, std::shared_ptr<material> _material): center(_center), radius(std::max(0.0, _radius)), mat(_material) {}

  bool hit(const ray& r, interval ray_interval, hit_record& rec) const override;
};

#endif