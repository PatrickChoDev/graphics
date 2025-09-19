#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <string>

#include <objects/color.hpp>
#include <objects/hittable.hpp>
#include <objects/vec3.hpp>

class camera {
public:
  double aspect_ratio = 1.0;
  int image_width = 100;
  int samples_per_pixel = 10;
  int max_depth = 10;
  point3 look_from = point3(0, 0, 0);
  point3 look_at = point3(0, 0, -1);
  vec3 v_up = vec3(0, 1, 0);
  double v_fov = 90.0;

  camera(std::string file_path): file_path(file_path) {}
  void render(const hittable& world);
private:
  std::string file_path;
  int image_height;
  double pixel_samples_scale;
  point3 camera_position;
  point3 upper_left_corner_pixel;
  vec3 pixel_delta_u, pixel_delta_v;

  void initialize();
  ray get_ray(int i, int j) const;
  vec3 sample_square() const;
  color get_ray_color(const ray&r, int depth, const hittable& world) const;
};

#endif