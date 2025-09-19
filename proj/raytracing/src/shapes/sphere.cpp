#include <cmath>

#include <shapes/sphere.hpp>

#include <objects/color.hpp>
#include <objects/interval.hpp>
#include <objects/hit_record.hpp>
#include <objects/vec3.hpp>

bool sphere::hit(const ray& r, interval ray_interval, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  double a = r.direction().length_squared();
  double half_b = dot(oc, r.direction());
  double c = oc.length_squared() - radius * radius;
  double discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return false;
  }

  double sqrt_discriminant = std::sqrt(discriminant);

  // Find the nearest root within the acceptable range.
  double root = (-half_b - sqrt_discriminant) / a;
  if (!ray_interval.surrounds(root)) {
    root = (-half_b + sqrt_discriminant) / a;
    if (!ray_interval.surrounds(root)) {
      return false;
    }
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);
  rec.mat = mat;

  return true;
}