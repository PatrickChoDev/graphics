#ifndef __VEC3_HPP__
#define __VEC3_HPP__

#include <iostream>

class vec3 {
public:
  double e[3];

  vec3(): e{0, 0, 0} {}
  vec3(double _e0, double _e1, double _e2): e{_e0, _e1, _e2} {}

  double x() const;
  double y() const;
  double z() const;

  vec3 operator-() const;
  double operator[](int i) const;
  double &operator[](int i);

  vec3 &operator+=(const vec3& v);
  vec3 &operator*=(const double t);
  vec3 &operator/=(const double t);

  double length() const;
  double length_squared() const;
  bool near_zero() const;
  
  static vec3 random();
  static vec3 random(double min, double max);
};

std::ostream& operator<<(std::ostream& out, const vec3& v);
vec3 operator+(const vec3& u, const vec3& v);
vec3 operator-(const vec3& u, const vec3& v);
vec3 operator*(const vec3& u, const vec3& v);
vec3 operator*(double t, const vec3& v);
vec3 operator*(const vec3& v, double t);
vec3 operator/(vec3 v, double t);
double dot(const vec3& u, const vec3& v);
vec3 cross(const vec3& u, const vec3& v);
vec3 unit_vector(vec3 v);

using point3 = vec3;

vec3 random_unit_vector();

#endif