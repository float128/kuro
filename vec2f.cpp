#ifndef KURO_VEC2F_CPP
#define KURO_VEC2F_CPP

#include <iostream>
#include <math.h>
#include "data-structures.h"
#include "vec2f.h"

using namespace kuro::math;
using namespace kuro::util;

vec2f::vec2f()
{
	x = 0.0;
	y = 0.0;
}

vec2f::vec2f(real a, real b)
{
	x = a;
	y = b;
}

vec2f operator + (vec2f a, vec2f b)
{
	return vec2f(a.x + b.x, a.y + b.y);
}

vec2f operator - (vec2f a, vec2f b)
{
	return vec2f(a.x - b.x, a.y - b.y);
}

vec2f operator * (real a, vec2f b)
{
	return vec2f(a * b.x, a * b.y);
}

vec2f operator * (vec2f a, real b)
{
	return vec2f(b * a.x, b * a.y);
}

vec2f operator / (vec2f a, real b)
{
	return vec2f(a.x / b, a.y / b);
}

real dot(vec2f a, vec2f b)
{
	return (a.x * b.x) + (a.y * b.y);
}

real len(vec2f a)
{
	return sqrt((a.x*a.x)+(a.y*a.y));
}

void print(vec2f a)
{
	cout<< "vec2f{ " << a.x <<", "<<a.y<<" }";
}

#endif
