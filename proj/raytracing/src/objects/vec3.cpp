#include <cmath>
#include <iostream>

#include <objects/vec3.hpp>
#include <randomizer.hpp>

// vec3 Method Definitions
double vec3::x() const {
  return e[0];
}

double vec3::y() const {
  return e[1];
}

double vec3::z() const {
  return e[2];
}

vec3 vec3::operator-() const {
  return vec3(-e[0], -e[1], -e[2]);
}

double vec3::operator[](int i) const {
  return e[i];
}

double &vec3::operator[](int i) {
  return e[i];
}

double vec3::length() const {
  return std::sqrt(length_squared());
}

double vec3::length_squared() const {
  return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

bool vec3::near_zero() const {
  double s = 1e-8;
  return (std::abs(e[0]) < s) && (std::abs(e[1]) < s) && (std::abs(e[2]) < s);
}

vec3 vec3::random() {
  return vec3(random_double(), random_double(), random_double());
}

vec3 vec3::random(double min, double max) {
  return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

// vec3 operator overloads
vec3 &vec3::operator+=(const vec3 &v) {
  e[0] += v.e[0];
  e[1] += v.e[1];
  e[2] += v.e[2];
  return *this;
}

vec3 &vec3::operator*=(const double t) {
  e[0] *= t;
  e[1] *= t;
  e[2] *= t;
  return *this;
}

vec3 &vec3::operator/=(const double t) {
  return *this *= 1 / t;
}

std::ostream& operator<< (std::ostream &out, const vec3& v) {
  return out << "(" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << ")";
}

vec3 operator+(const vec3& u, const vec3& v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

vec3 operator-(const vec3& u, const vec3& v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

vec3 operator*(const vec3& u, const vec3& v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

vec3 operator*(double t, const vec3& v) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

vec3 operator*(const vec3& v, double t) {
  return t * v;
}

vec3 operator/(vec3 v, double t) {
  return (1 / t) * v;
}

// vec3 utility functions
double dot(const vec3& u, const vec3& v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

vec3 cross(const vec3& u, const vec3& v) {
  return vec3(
    u.e[1] * v.e[2] - u.e[2] * v.e[1],
    u.e[2] * v.e[0] - u.e[0] * v.e[2],
    u.e[0] * v.e[1] - u.e[1] * v.e[0]
  );
}

vec3 unit_vector(vec3 v) {
  double len = v.length();
  if (len == 0) {
    return v;
  }
  return v / len;
}

vec3 random_unit_vector() {
  while(true) {
    vec3 p = vec3::random(-1, 1);
    double p_length_squared = p.length_squared();

    if(1e-160 < p_length_squared && p_length_squared < 1) {
      return p / std::sqrt(p_length_squared);
    }
  }
}