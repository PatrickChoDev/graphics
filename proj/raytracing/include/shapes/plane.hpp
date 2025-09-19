#ifndef __PLANE_HPP__
#define __PLANE_HPP__

#include <memory>
#include <cmath>

#include <objects/hittable.hpp>
#include <objects/vec3.hpp>
#include <objects/ray.hpp>
#include <objects/hit_record.hpp>
#include <materials/base.hpp>

// Infinite plane defined by a point and a (normalized) surface normal.
// Equation: dot(normal, (P - point)) = 0
class plane : public hittable {
public:
  point3 p0;                       // A point on the plane
  vec3 n;                          // Outward normal (kept normalized)
  std::shared_ptr<material> mat;   // Material of the plane

  plane(const point3& point, const vec3& normal, std::shared_ptr<material> m)
    : p0(point), n(unit_vector(normal)), mat(std::move(m)) {}

  bool hit(const ray& r, interval ray_interval, hit_record& rec) const override {
    double denom = dot(n, r.direction());
    // If denom is near zero, the ray is parallel to the plane (no hit)
    const double EPS = 1e-8;
    if (std::fabs(denom) < EPS) {
      return false;
    }

    double t = dot(p0 - r.origin(), n) / denom;
    if (!ray_interval.surrounds(t)) {
      return false;
    }

    rec.t = t;
    rec.p = r.at(t);
    rec.mat = mat;
    // Plane has fixed outward normal n; set face normal adjusts orientation
    rec.set_face_normal(r, n);

    return true;
  }
};

#endif