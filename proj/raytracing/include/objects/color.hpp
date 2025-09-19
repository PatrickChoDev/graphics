#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <cmath>

#include <objects/vec3.hpp>

using color = vec3;

double linear_to_gamma(double linear);
color get_color_byte(color c);

#endif