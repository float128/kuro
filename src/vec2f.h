#ifndef KURO_VEC2F_H
#define KURO_VEC2F_H

#include "data-structures.h"

using namespace kuro::math;
using namespace kuro::util;

/*used to hold a 2d position*/
struct kuro::math::vec2f
{
	vec2f();
	vec2f(real a, real b);
	real x, y;
};

/*operator to add 2d floating point vectors*/
vec2f operator + (vec2f a, vec2f b);

/*operator to subtract 2d floating point vectors*/
vec2f operator - (vec2f a, vec2f b);

/*operators to multiply a 2d floating point vector by a constant*/
vec2f operator * (real a, vec2f b);
vec2f operator * (vec2f a, real b);

/*operators to divide a 2d floating point vector by a constant*/
vec2f operator / (vec2f a, real b);

/*returns the dot product of two 2d vectors*/
real dot(vec2f a, vec2f b);

/*returns the length of a 2d vector*/
real len(vec2f a);

/*prints a 2d vector*/
void print(vec2f a);

#endif
