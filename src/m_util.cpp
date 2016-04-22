#ifndef KURO_M_UTIL_CPP
#define KURO_M_UTIL_CPP

#include <iostream>

#include "data-structures.h"
#include "vec4f.h"
#include "mat4f.h"

vec4f kuro::math::m_util::barycentric(vec4f a, vec4f b, vec4f c, vec4f x)
{
	vec4f ba = b - a;
	vec4f ca = c - a;
	
	/*works out the edges of the triangle*/
	vec4f xa = x - a;
	vec4f xb = x - b;
	vec4f xc = x - c;

	return vec4f(len(cross(xb, xc)), len(cross(xa, xc)), len(cross(xa, xb))) / len(cross(ba, ca));
}

#endif
