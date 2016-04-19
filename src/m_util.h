#ifndef KURO_M_UTIL_H
#define KURO_M_UTIL_H


#include "data-structures.h"
#include "vec4f.h"
#include "mat4f.h"

vec4f barycentric(vec4f a, vec4f b, vec4f c, vec4f x)
{
	vec4f ba = b - a;
	vec4f ca = c - a;

	vec4f xa = x - a;
	vec4f xb = x - b;
	vec4f xc = x - c;

	return vec4f(len(cross(xb, xc)), len(cross(xa, xc)), len(cross(xa, xb))) / len(cross(ba, ca));
}

#endif
