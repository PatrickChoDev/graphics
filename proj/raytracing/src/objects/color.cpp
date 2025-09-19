#include <cmath>

#include <objects/interval.hpp>
#include <objects/color.hpp>
#include <objects/vec3.hpp>

// color utility function definitions
double linear_to_gamma(double linear) {
  if(linear > 0) {
    return std::sqrt(linear);
  }
  return 0;
}

color get_color_byte(color c) {
  // Apply gamma correction (gamma = 2.0) and output integer values in [0,255] per PPM spec
  interval intensity(0.0, 0.999);

  double r = intensity.clamp(linear_to_gamma(c.x()));
  double g = intensity.clamp(linear_to_gamma(c.y()));
  double b = intensity.clamp(linear_to_gamma(c.z()));

  int ir = static_cast<int>(256 * r);
  int ig = static_cast<int>(256 * g);
  int ib = static_cast<int>(256 * b);

  return color(ir, ig, ib);
}