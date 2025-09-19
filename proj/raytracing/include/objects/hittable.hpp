#ifndef __HITTABLE_HPP__
#define __HITTABLE_HPP__

#include <memory>
#include <vector>

#include <objects/hit_record.hpp>
#include <objects/interval.hpp>
#include <objects/ray.hpp>

class hittable {
public:
  virtual ~hittable() = default;
  virtual bool hit(const ray& r, interval ray_interval, hit_record& rec) const = 0;
};

class hittable_list: public hittable {
public:
  std::vector<std::shared_ptr<hittable>> objects;

  hittable_list() {}
  hittable_list(std::shared_ptr<hittable> object);
  void clear();
  void add(std::shared_ptr<hittable> object);
  bool hit(const ray& r, interval ray_interval, hit_record& rec) const override;
};

#endif