#include <iostream>
#include <memory>

#include <objects/camera.hpp>
#include <objects/color.hpp>
#include <objects/hittable.hpp>

#include <shapes/sphere.hpp>
#include <shapes/box.hpp>

#include <materials/lambertian.hpp>
#include <materials/metal.hpp>
#include <materials/dielectric.hpp>

signed main() {
  hittable_list world;

  auto material_ground = std::make_shared<lambertian>(color(0.11, 0.14, 0.22));
  auto material_center = std::make_shared<lambertian>(color(0.9, 0.1, 0.1));
  auto material_side = std::make_shared<metal>(color(1.0, 1.0, 1.0), 0.0);
  auto material_glass = std::make_shared<dielectric>(1.5);

  // Scene objects: glass center, metals, small diffuse sphere, ground sphere, and background box
      world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5, material_glass));
// Removed right sphere at (1, 0, -1)
      world.add(std::make_shared<sphere>(point3(-1, 0, -1), 0.5, material_side));
      world.add(std::make_shared<sphere>(point3(0, -0.25, -2), 0.25, material_center));
      world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
      // world.add(std::make_shared<box>(point3(-1.25, -0.5, -3.25), point3(1.25, 1.25, -2.25), material_center));
      // world.add(std::make_shared<box>(point3(0.3, -0.2, -3.6), point3(1.0, 0.8, -2.8), material_center));
      world.add(std::make_shared<box>(point3(0.5, -0.25, -3.5), point3(5.0, 0.35, -2.9), material_center));


  camera cam("./images/out.ppm");

  // adjust camera parameters here
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 1920;
  cam.samples_per_pixel = 100;
  cam.max_depth = 5;

  cam.render(world);

  std::cout << "\nImage rendered to ./images/out.ppm" << std::endl;

  return 0;
}
