#include <materials/metal.hpp>
// reflect helper is intentionally defined with external linkage so other materials (e.g. dielectric) can reuse it.
// Consider moving this to a shared header (e.g. a vec_utils.hpp) if broader reuse is needed.

vec3 reflect(const vec3& v, const vec3& n) {
  // Reflection of v about normal n. Assumes n is normalized.
  return v - 2 * dot(v, n) * n;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
  vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
  reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
  scattered = ray(rec.p, reflected);
  attenuation = albedo;
  return dot(scattered.direction(), rec.normal) > 0;
}