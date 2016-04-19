

#ifndef KURO_TRANSFORM_CPP
#define KURO_TRANSFORM_CPP

#include <math.h>

#include "data-structures.h"
#include "vec4f.h"
#include "mat4f.h"

kuro::math::vec4f kuro::math::transform::translate(vec4f p, vec4f delta)
{
    return p + delta;
}

kuro::math::mat4f kuro::math::transform::translate(vec4f delta)
{
    mat4f M = {1.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 1.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 1.0f, 0.0f,
               delta.x, delta.y, delta.z, 1.0f};

    return M;
}

kuro::math::mat4f kuro::math::transform::rotate(vec4f axis, kuro::util::real theta)
{
    theta = (theta/180.0f)* kuro::math::PI;

    axis = normalise(axis);

    kuro::util::real c = cos(theta);
    kuro::util::real s = sin(theta);

    mat4f M = {c + ((axis.x*axis.x)*(1.0f - c)),
               (axis.y*axis.x*(1.0f - c)) + (axis.z * s),
               (axis.z*axis.x*(1.0f - c)) - (axis.y * s),
               0.0f,
               (axis.x*axis.y*(1.0f - c)) - (axis.z * s),
               c + ((axis.y*axis.y)*(1.0f - c)),
               (axis.z*axis.y*(1.0f - c)) + (axis.x * s),
               0.0f,
               (axis.x*axis.z*(1.0f - c)) + (axis.y * s),
               (axis.y*axis.z*(1.0f - c)) - (axis.x * s),
               c + ((axis.z*axis.z)*(1.0f - c)),
               0.0f,
               0.0f, 0.0f, 0.0f, 1.0f};

/*
    mat4f M = {c + ((axis.x*axis.x)*(1.0f - c)),
               (axis.x*axis.y*(1.0f - c)) - (axis.z * s),
               (axis.x*axis.z*(1.0f - c)) - (axis.y * s),
               0.0f,
               (axis.y*axis.x*(1.0f - c)) - (axis.z * s),
               c + ((axis.y*axis.y)*(1.0f - c)),
               (axis.y*axis.z*(1.0f - c)) - (axis.x * s),
               0.0f,
               (axis.z*axis.x*(1.0f - c)) - (axis.y * s),
               (axis.z*axis.y*(1.0f - c)) - (axis.x * s),
               c + ((axis.z*axis.z)*(1.0f - c)),
               0.0f,
               0.0f, 0.0f ,0.0f, 1.0f};
*/
    return M;
}

kuro::math::vec4f kuro::math::transform::scale(vec4f p, vec4f scale_factor)
{
    return vec4f(p.x * scale_factor.x,
                 p.y * scale_factor.y,
                 p.z * scale_factor.z,
                 p.w * scale_factor.w);
}

kuro::math::mat4f kuro::math::transform::scale(vec4f scale_factor)
{
    mat4f M = {scale_factor.x, 0.0f, 0.0f, 0.0f,
               0.0f, scale_factor.y, 0.0f, 0.0f,
               0.0f, 0.0f, scale_factor.z, 0.0f,
               0.0f, 0.0f, 0.0f, 1.0f};

    return M;
}

#endif
