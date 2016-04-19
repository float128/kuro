#ifndef KURO_RGBA_CPP
#define KURO_RGBA_CPP

#include "data-structures.h"

#include "rgba.h"

using namespace kuro::util;
using namespace kuro::math;

rgba operator*(rgba colour, real c)
{
    return rgba(colour.r * c,
                colour.g * c,
                colour.b * c,
                colour.a * c);
}

rgba operator*(real c, rgba colour)
{
    return rgba(colour.r * c,
                colour.g * c,
                colour.b * c,
                colour.a * c);
}

rgba operator+(rgba c1, rgba c2)
{
    return rgba(c1.r + c2.r,
                c1.g + c2.g,
                c1.b + c2.b,
                c1.a + c2.a);
}

void print(rgba col)
{
	cout<<  "rgba( "<< col.r <<", "
				<< col.g <<", "
				<< col.b <<", "
				<< col.a
				<<" )";
}


#endif
