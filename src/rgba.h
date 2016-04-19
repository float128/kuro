#ifndef KURO_RGBA_H
#define KURO_RGBA_H

#include "data-structures.h"
#include <iostream>

using namespace std;
using namespace kuro::util;
using namespace kuro::math;

/*used holds RGB values*/
struct kuro::util::rgba
{
	rgba(real in_r = 0.0, real in_g = 0.0, real in_b = 0.0, real in_a = 0.0)
	{
		r = in_r;
		g = in_g;
		b = in_b;
		a = in_a;
	}

	real r;
	real g;
	real b;
	real a;
};

/*multiplies the components by a constant*/
rgba operator*(rgba colour, real c);
rgba operator*(real c, rgba colour);

/*adds the components of two RGBA colour values*/
rgba operator+(rgba c1, rgba c2);

/*prints rgba value*/
void print(rgba col);

#endif
