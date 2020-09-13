#ifndef KURO_RAY4F_CPP
#define KURO_RAY4F_CPP

#include <iostream>

#include "data-structures.h"
#include "ray4f.h"
#include "vec4f.h"

using namespace kuro::util;
using namespace kuro::math;

void print(ray4f r)
{
	std::cout<< "ray4f[ ";

	print(r.p);
	std::cout<<", ";
	print(r.d);

	std::cout<< " ]";
}

#endif
