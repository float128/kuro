#ifndef KURO_VEC4F_CPP
#define KURO_VEC4F_CPP

#include <iostream>
#include <math.h>
#include <cmath>
#include "data-structures.h"
#include "vec4f.h"

using namespace kuro::util;
using namespace kuro::math;

vec4f::vec4f(real in_x, real in_y, real in_z, real in_w)
{
	x = in_x;
	y = in_y;
	z = in_z;
	w = in_w;
}

vec4f operator + (vec4f a, vec4f b)
{
	return vec4f(a.x + b.x,
				 a.y + b.y,
				 a.z + b.z,
				 a.w + b.w);
}

vec4f operator - (vec4f a, vec4f b)
{
	return vec4f(a.x - b.x,
				 a.y - b.y,
				 a.z - b.z,
				 a.w - b.w);
}

vec4f operator * (real a, vec4f b)
{
	return vec4f(a * b.x,
				 a * b.y,
				 a * b.z,
				 a * b.w);
}

vec4f operator * (vec4f a, real b)
{
	return vec4f(a.x * b,
				 a.y * b,
				 a.z * b,
				 a.w * b);
}

vec4f operator / (vec4f a, real b)
{
	return vec4f(a.x / b,
				 a.y / b,
				 a.z / b,
				 a.w / b);
}

real dot(vec4f a, vec4f b)
{
	return (a.x * b.x) +
		   (a.y * b.y) +
		   (a.z * b.z) +
		   (a.w * b.w);
}

vec4f cross(vec4f a, vec4f b)
{
	return vec4f((a.y * b.z) - (a.z * b.y),
				 (a.z * b.x) - (a.x * b.z),
				 (a.x * b.y) - (a.y * b.x));
}

real len(vec4f a)
{
	return sqrt((a.x * a.x) +
				(a.y * a.y) +
				(a.z * a.z) +
				(a.w * a.w));
}

vec4f normalise(vec4f a)
{
	return a / len(a);
}

vec4f abs(vec4f a)
{
    return vec4f(fabs(a.x),
                 fabs(a.y),
                 fabs(a.z),
                 fabs(a.w));
}

void print(vec4f a)
{
	cout<< "vec4f{ " << a.x << ", " << a.y << ", " << a.z << ", " << a.w << " }";
}

#endif
