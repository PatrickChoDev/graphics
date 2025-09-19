#ifndef __HIT_RECORD_HPP__
#define __HIT_RECORD_HPP__

#include <memory>
#include <objects/color.hpp>
#include <objects/ray.hpp>
#include <objects/vec3.hpp>

class material;

class hit_record {
public:
  point3 p;
  vec3 normal;
  double t;
  bool front_face;
  std::shared_ptr<material> mat;
  
  void set_face_normal(const ray& r, const vec3& outward_normal);
};

#endif