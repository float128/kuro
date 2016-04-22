#ifndef KURO_OBJECT_CPP
#define KURO_OBJECT_CPP

#include <math.h>
#include <iostream>
#include "data-structures.h"
#include "util.h"
#include "vec2f.h"
#include "vec4f.h"
#include "ray4f.h"
#include "object.h"

using namespace std;
using namespace kuro;
using namespace kuro::util;
using namespace kuro::math;

idata object::sphere::intersect(ray4f ir)
{
    idata data;
    data.hit_object = this;
    data.hit = false;

    vec4f s = ir.p - p;
    vec4f d = ir.d;
    real a = 2.0 * (dot(d, s)/dot(d, d));
    real b = (dot(s, s) - (r*r))/dot(d, d);
    real discr = (a*a) - (4*b);

    if(discr < 0)return data;

    real x1 = 0.5 * (-a + sqrt(discr));
    real x2 = 0.5 * (-a - sqrt(discr));
    real x = 0.0;

    if((x1 >= 0)&&((x1 < x2)||(x2 <= 0)))
    {
        data.hit = true;
        x = x1;
    }

    if((x2 >= 0)&&((x2 < x1)||(x1 <= 0)))
    {
        data.hit = true;
        x = x2;
    }

    if(data.hit)
    {
        data.p = ir.p + (ir.d * x);
        data.n = normalise(data.p-p);
    }

    return data;
}

idata object::disk::intersect(ray4f ir)
{
    idata data;
    data.hit_object = this;
    data.hit = false;
    data.n = n;

    real x = dot(p - ir.p, n)/dot(ir.d, n);
    data.p = ir.p + (ir.d * x);

    if( (x >=0.0) && (len(data.p - p)<=r) )data.hit = true;

    return data;
}

idata object::line::intersect(ray4f ir)
{
    idata data;
    data.hit_object = this;
    data.hit = false;

    vec4f p0 = ir.p;
    vec4f p1 = p;
    vec4f u = ir.d;
    vec4f v = d;


    vec2f solutions;

    if(dot(u, v) == 0)
    {
        solutions.x = dot(u, p1 - p0)/dot(u, u);
        solutions.y = dot(v, p0 - p1)/dot(v, v);
    }
    else
    {
        solutions = linear_solve(dot(u, u),
                            -dot(u, v),
                             dot(v, u),
                            -dot(v, v),
                             dot(u, p1 - p0),
                             dot(v, p1 - p0));
    }

    vec4f dx = (p0 + (u*solutions.x)) - (p1 + (v*solutions.y));

    if((len(dx) <= r)&&(solutions.x >= 0)&&(solutions.y >= 0))
    {
        data.hit = true;
        data.p = p1 + (v * solutions.y);
        data.n = normalise(dx);
    }

    return data;
}

vec2f linear_solve(real a, real b, real c, real d, real u, real v)
{
    vec2f s;
	/*solves a simulataneous equation of two variables
	and stores the results in s.x and s.y*/
    s.x = v - ((d*u)/b);
    s.x /= c - ((d*a)/b);
    s.y = (u - (a*s.x))/b;

    return s;
}

#endif // KURO_OBJECT_H
