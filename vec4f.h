#ifndef KURO_VEC4F_H
#define KURO_VEC4F_H

#include "data-structures.h"

using namespace kuro::math;
using namespace kuro::util;

/*used to hold a 3d/4d position or direction*/
struct kuro::math::vec4f
{
	vec4f(real in_x = 0.0, real in_y = 0.0, real in_z = 0.0, real in_w = 0.0);
	real x, y, z, w;
};

/*returns the addition of two 4d vectors*/
vec4f operator + (vec4f a, vec4f b);

/*returns the subtraction of two 4d vectors*/
vec4f operator - (vec4f a, vec4f b);

/*returns the multiplication of a 4d vector by a scalar*/
vec4f operator * (real a, vec4f b);
vec4f operator * (vec4f a, real b);

/*returns the division of a 4d vector by a scalar*/
vec4f operator / (vec4f a, real b);

/*returns the dot product of two 4d vectors*/
real dot(vec4f a, vec4f b);

/*returns the dot product of two 3d vectors (ignoring the w component)*/
vec4f cross(vec4f a, vec4f b);

/*returns the length of a 4d vector*/
real len(vec4f a);

/*returns the normalised vector of the input vector*/
vec4f normalise(vec4f a);

/*converts negative components to positive ones*/
vec4f abs(vec4f a);

/*prints a 4d vector*/
void print(vec4f a);

#endif
