#ifndef __RAY_HPP__
#define __RAY_HPP__

#include <objects/vec3.hpp>

class ray {
private:
  point3 orig;
  vec3 dir;
public:
  ray() {}
  ray(const point3& _orig, const vec3& _dir): orig(_orig), dir(_dir) {}
  
  const point3& origin() const;
  const vec3& direction() const;

  point3 at(double t) const;
};

#endif