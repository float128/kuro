#ifndef KURO_UTIL_CPP
#define KURO_UTIL_CPP

#include <random>
#include "data-structures.h"
#include "vec4f.h"
#include "util.h"

using namespace kuro::util;
using namespace kuro::math;

real random::real_random()
{
    return distribution(generator);
}

vec4f random::vec3f_random(vec4f n)
{
    real alpha = 2 * PI * random::real_random();
    real beta =  2 * PI * random::real_random();

    vec4f d;
    vec4f i;
    vec4f j;

    i.x = cos(alpha);
    i.z = sin(alpha);
    i.y = 0.0;
    i.w = 0.0;

    j = vec4f(0.0, 1.0, 0.0, 0.0);

    d = d + (i * cos(beta));
    d = d + (j * sin(beta));

    if(dot(d, n) >= 0.0)return d;

    return (-1.0) * d;
}

#endif // KURO_UTIL_IDATA
