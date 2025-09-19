#include <algorithm>
#include <fstream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

#include <objects/camera.hpp>
#include <objects/color.hpp>
#include <objects/hittable.hpp>

#include <materials/base.hpp>

#include <constants.hpp>
#include <progress.hpp>
#include <randomizer.hpp>

// camera method definitions
void camera::render(const hittable& world) {
  initialize();

  const int total_pixels = image_width * image_height;
  progress_bar progress(total_pixels);

  // Framebuffer to store linear color (before gamma + byte conversion)
  std::vector<color> framebuffer(total_pixels, color(0,0,0));

  // Multithreading setup
  const unsigned hw = std::thread::hardware_concurrency();
  int thread_count = hw == 0 ? 4 : static_cast<int>(hw);
  std::atomic<int> next_row{0};
  std::atomic<int> pixels_done{0};
  std::atomic<bool> workers_finished{false};

  // Separate monitor thread to update progress safely (avoids data races in progress_bar)
  std::thread monitor([&](){
    int last_reported = 0;
    while(!workers_finished.load()) {
      int done = pixels_done.load();
      // Update progress for new completed pixels
      while(last_reported < done) {
        progress.update();
        ++last_reported;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    // Flush remaining
    int done = pixels_done.load();
    while(last_reported < done) {
      progress.update();
      ++last_reported;
    }
    progress.finish();
  });

  auto worker = [&](int /*id*/) {
    while(true) {
      int i = next_row.fetch_add(1);
      if(i >= image_height) break;
      for(int j=0; j<image_width; ++j) {
        color pixel_color(0,0,0);
        for(int sample=0; sample<samples_per_pixel; ++sample) {
          ray r = get_ray(i, j);
          pixel_color += get_ray_color(r, max_depth, world);
        }
        pixel_color *= pixel_samples_scale;
        framebuffer[i * image_width + j] = pixel_color;
        pixels_done.fetch_add(1, std::memory_order_relaxed);
      }
    }
  };

  std::vector<std::thread> threads;
  threads.reserve(thread_count);
  for(int t=0; t<thread_count; ++t) {
    threads.emplace_back(worker, t);
  }
  for(auto& th : threads) {
    th.join();
  }
  workers_finished = true;
  monitor.join();

  // Write PPM after rendering completes
  std::ofstream file_out(file_path);
  file_out << "P3\n";
  file_out << image_width << " " << image_height << "\n" << 255 << "\n";
  for(int i=0; i<image_height; ++i) {
    for(int j=0; j<image_width; ++j) {
      color c = get_color_byte(framebuffer[i * image_width + j]);
      file_out << c.x() << " " << c.y() << " " << c.z() << "\n";
    }
  }
}

void camera::initialize() {
  image_height = std::max(1, static_cast<int>(image_width / aspect_ratio));
  pixel_samples_scale = 1.0 / samples_per_pixel;

  // Vertical field-of-view in degrees to radians
  double theta = v_fov * PI / 180.0;
  double h = std::tan(theta / 2.0);
  double viewport_height = 2.0 * h;
  double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

  vec3 w = unit_vector(look_from - look_at);
  vec3 u = unit_vector(cross(v_up, w));
  vec3 v = cross(w, u);

  camera_position = look_from;
  vec3 horizontal = viewport_width * u;
  vec3 vertical = viewport_height * v;
  upper_left_corner_pixel = camera_position - w - horizontal / 2 + vertical / 2;
  pixel_delta_u = horizontal / image_width;
  pixel_delta_v = -vertical / image_height;
  upper_left_corner_pixel += 0.5 * (pixel_delta_u + pixel_delta_v);
}

ray camera::get_ray(int i, int j) const {
  vec3 offset = sample_square();
  vec3 pixel_sample = upper_left_corner_pixel + ((offset.x() + j) * pixel_delta_u) + ((offset.y() + i) * pixel_delta_v);

  vec3 ray_origin = camera_position;
  vec3 ray_direction = pixel_sample - ray_origin;

  return ray(ray_origin, ray_direction);
}

vec3 camera::sample_square() const {
  return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

color camera::get_ray_color(const ray& r, int depth, const hittable& world) const {
  if (depth <= 0) {
    return color(0,0,0);
  }

  hit_record rec;
  if (world.hit(r, interval(0.001, INF), rec)) {
    ray scattered;
    color attenuation;
    if (rec.mat && rec.mat->scatter(r, rec, attenuation, scattered)) {
      return attenuation * get_ray_color(scattered, depth - 1, world);
    }
    return color(0,0,0);
  }

  vec3 unit_direction = unit_vector(r.direction());
  double t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}