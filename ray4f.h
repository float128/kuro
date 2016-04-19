#ifndef KURO_RAY4F_H
#define KURO_RAY4F_H

#include "data-structures.h"
#include "vec4f.h"

using namespace kuro::util;
using namespace kuro::math;

/*used to represent a 3d line from a given position
in a given direction*/
struct kuro::math::ray4f
{
	ray4f(vec4f in_p = vec4f(), vec4f in_d = vec4f())
	{
		p = in_p;
		d = in_d;
	}

	vec4f p;
	vec4f d;
};

/*prints ray data structure*/
void print(ray4f r);

#endif
