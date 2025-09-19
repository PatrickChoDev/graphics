#ifndef __BOX_HPP__
#define __BOX_HPP__

#include <cmath>
#include <limits>
#include <memory>

#include <objects/hittable.hpp>
#include <objects/ray.hpp>
#include <objects/vec3.hpp>
#include <objects/hit_record.hpp>
#include <objects/interval.hpp>
#include <materials/base.hpp>

// Axis-Aligned Bounding Box (AABB) primitive as a hittable.
// Defined by its minimum and maximum corner points in 3D space.
// This is a closed box: points on the boundary are considered inside.
//
// Intersection algorithm (slab method):
// For each axis, compute the parametric entering and exiting t values.
// Maintain the running interval of intersection across all three axes.
// If after processing all axes the interval is valid (t_max > t_min) and
// overlaps the requested ray_interval, we have a hit.
//
// The normal is determined from which axis produced the final entering t.
class box : public hittable {
public:
  point3 min_corner;
  point3 max_corner;
  std::shared_ptr<material> mat;

  box() = default;

  box(const point3& min_c, const point3& max_c, std::shared_ptr<material> m)
    : min_corner(min_c), max_corner(max_c), mat(std::move(m)) {
    // Ensure ordering (in case user swapped inputs).
    for (int i = 0; i < 3; ++i) {
      if (min_corner.e[i] > max_corner.e[i]) {
        std::swap(min_corner.e[i], max_corner.e[i]);
      }
    }
  }

  // Convenience constructor: center + size (uniform)
  box(const point3& center, double extent, std::shared_ptr<material> m) : mat(std::move(m)) {
    double h = extent * 0.5;
    min_corner = point3(center.x() - h, center.y() - h, center.z() - h);
    max_corner = point3(center.x() + h, center.y() + h, center.z() + h);
  }

  bool hit(const ray& r, interval ray_interval, hit_record& rec) const override {
    // For numerical robustness: if direction component is 0, treat as very close to 0
    // but we explicitly handle the parallel case.

    double t_enter = ray_interval.min;
    double t_exit  = ray_interval.max;

    int enter_axis = -1;

    for (int axis = 0; axis < 3; ++axis) {
      double origin = r.origin()[axis];
      double direction = r.direction()[axis];
      double inv_dir;

      if (std::fabs(direction) < 1e-15) {
        // Ray parallel to slab: must be within slab range or miss entirely.
        if (origin < min_corner[axis] || origin > max_corner[axis]) {
          return false;
        }
        // No interval refinement from this axis.
        continue;
      } else {
        inv_dir = 1.0 / direction;
      }

      double t0 = (min_corner[axis] - origin) * inv_dir;
      double t1 = (max_corner[axis] - origin) * inv_dir;
      if (t0 > t1) std::swap(t0, t1);

      // Update global entering and exiting times
      if (t0 > t_enter) {
        t_enter = t0;
        enter_axis = axis;
      }
      t_exit = std::fmin(t_exit, t1);

      if (t_enter > t_exit) {
        return false; // Slabs do not overlap
      }
    }

    // Validate final intersection interval with the provided ray_interval
    if (!ray_interval.surrounds(t_enter)) {
      // Try the exit point if the entering point is out-of-range and still inside interval
      if (!ray_interval.surrounds(t_exit)) {
        return false;
      }
      // Hitting from inside the box; treat t_exit as the outward hit.
      rec.t = t_exit;
    } else {
      rec.t = t_enter;
    }

    rec.p = r.at(rec.t);
    rec.mat = mat;

    // Determine outward normal from enter_axis and sign of direction component
    vec3 outward_normal(0, 0, 0);

    if (enter_axis == -1) {
      // Degenerate case (ray started inside). Choose the axis with the largest penetration.
      // Compute distances to each face and pick smallest absolute distance.
      double dx = std::min(std::fabs(rec.p.x() - min_corner.x()), std::fabs(max_corner.x() - rec.p.x()));
      double dy = std::min(std::fabs(rec.p.y() - min_corner.y()), std::fabs(max_corner.y() - rec.p.y()));
      double dz = std::min(std::fabs(rec.p.z() - min_corner.z()), std::fabs(max_corner.z() - rec.p.z()));
      if (dx <= dy && dx <= dz) {
        outward_normal = (std::fabs(rec.p.x() - min_corner.x()) < std::fabs(max_corner.x() - rec.p.x()))
                           ? vec3(-1, 0, 0) : vec3(1, 0, 0);
      } else if (dy <= dz) {
        outward_normal = (std::fabs(rec.p.y() - min_corner.y()) < std::fabs(max_corner.y() - rec.p.y()))
                           ? vec3(0, -1, 0) : vec3(0, 1, 0);
      } else {
        outward_normal = (std::fabs(rec.p.z() - min_corner.z()) < std::fabs(max_corner.z() - rec.p.z()))
                           ? vec3(0, 0, -1) : vec3(0, 0, 1);
      }
    } else {
      // Assign normal along enter_axis
      int axis = enter_axis;
      double dir_component = r.direction()[axis];
      // If ray direction is positive, we hit the min face; else the max face (for t_enter)
      // but because we set t based on entering slab, we invert logic carefully.
      // For numeric stability we look at rec.p relative to box faces.
      switch (axis) {
        case 0: // x
          outward_normal = (std::fabs(rec.p.x() - min_corner.x()) < 1e-8) ? vec3(-1, 0, 0)
                                                                          : ((std::fabs(rec.p.x() - max_corner.x()) < 1e-8) ? vec3(1, 0, 0)
                                                                                                                            : (dir_component > 0 ? vec3(-1,0,0) : vec3(1,0,0)));
          break;
        case 1: // y
          if (std::fabs(rec.p.y() - min_corner.y()) < 1e-8) {
            outward_normal = vec3(0, -1, 0);
          } else if (std::fabs(rec.p.y() - max_corner.y()) < 1e-8) {
            outward_normal = vec3(0, 1, 0);
          } else {
            outward_normal = (dir_component > 0 ? vec3(0, -1, 0) : vec3(0, 1, 0));
          }
          break;
        case 2: // z
          outward_normal = (std::fabs(rec.p.z() - min_corner.z()) < 1e-8) ? vec3(0, 0, -1)
                                                                          : ((std::fabs(rec.p.z() - max_corner.z()) < 1e-8) ? vec3(0, 0, 1)
                                                                                                                            : (dir_component > 0 ? vec3(0,0,-1) : vec3(0,0,1)));
          break;
      }
    }

    rec.set_face_normal(r, outward_normal);
    return true;
  }
};

#endif