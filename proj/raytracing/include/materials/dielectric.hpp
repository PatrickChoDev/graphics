#ifndef __DIELECTRIC_HPP__
#define __DIELECTRIC_HPP__

#include <cmath>
#include <materials/base.hpp>
#include <randomizer.hpp>

// Dielectric (glass-like) material.
// Uses Snell's law for refraction and Schlick's approximation for reflectance.
class dielectric : public material {
public:
  // Index of refraction (e.g. glass ~1.5, water ~1.33, diamond ~2.4)
  double ir;

  explicit dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
    attenuation = color(1.0, 1.0, 1.0); // No absorption (perfectly clear)

    // Figure out if the ray is entering or exiting the material
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_dir = unit_vector(r_in.direction());

    double cos_theta = std::fmin(dot(-unit_dir, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    vec3 direction;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
      direction = reflect(unit_dir, rec.normal);
    } else {
      direction = refract(unit_dir, rec.normal, refraction_ratio);
    }

    scattered = ray(rec.p, direction);
    return true;
  }

private:
  static vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
  }

  static vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    double cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
  }

  // Schlick's approximation for reflectance
  static double reflectance(double cosine, double ref_idx) {
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5);
  }
};

#endif