#ifndef KURO_TRIANGLE_CPP
#define KURO_TRIANGLE_CPP

#include <vector>
#include <stdio.h>
#include "data-structures.h"
#include "vec4f.h"
#include "object.h"
#include "ray4f.h"
#include "util.h"
#include "vec2f.h"
#include "vec4f.h"
#include "object.h"
#include "triangle.h"

kuro::util::idata kuro::object::triangle::intersect(kuro::math::ray4f ir, vec4f* vertices)
{
    kuro::util::idata data;
    data.hit_object = NULL;
    data.hit = false;

    kuro::math::vec4f origin = vertices[point[0]];

    kuro::math::vec4f u = vertices[point[1]] - vertices[point[0]];
    kuro::math::vec4f v = vertices[point[2]] - vertices[point[0]];

    kuro::math::vec4f n = cross(u, v);

    kuro::util::real x = dot(origin - ir.p, n)/dot(ir.d, n);

    data.distance = x;

    if(x >= 0.0)
    {
        data.p = ir.p + (ir.d * x);
        data.n = n;

        kuro::math::vec2f uv = uv_coords(origin, u, v, data.p);

        if((uv.x >= 0)&&(uv.y >= 0)&&(uv.x + uv.y <= 1.0))
        {
            data.hit = true;
        }
    }

    return data;
}

kuro::util::idata kuro::object::triangle::intersect(kuro::math::ray4f ir, vec4f* vertices, vec4f* vnormals)
{
    kuro::util::idata data;
    data.hit_object = NULL;
    data.hit = false;

    kuro::math::vec4f origin = vertices[point[0]];

    kuro::math::vec4f u = vertices[point[1]] - vertices[point[0]];
    kuro::math::vec4f v = vertices[point[2]] - vertices[point[0]];

    kuro::math::vec4f n = cross(u, v);

    kuro::util::real x = dot(origin - ir.p, n)/dot(ir.d, n);

    data.distance = x;

    if(x >= 0.0)
    {
        data.p = ir.p + (ir.d * x);
        data.n = n;

        vec4f bryc_coords = kuro::math::m_util::barycentric(vertices[point[0]],
															vertices[point[1]],
															vertices[point[2]],
															data.p);

        if(bryc_coords.x + bryc_coords.y + bryc_coords.z <= 1.0001)
        {
			data.n = (bryc_coords.x * vnormals[vnormal[0]]) +
					 (bryc_coords.y * vnormals[vnormal[1]]) +
					 (bryc_coords.z * vnormals[vnormal[2]]);

            data.hit = true;
        }
    }

    return data;
}

kuro::math::vec2f uv_coords(kuro::math::vec4f origin,
                            kuro::math::vec4f u,
                            kuro::math::vec4f v,
                            kuro::math::vec4f p)
{
    vec4f c = p - origin;

    vec2f uv = linear_solve(dot(u, u),
                            dot(u, v),
                            dot(u, v),
                            dot(v, v),
                            dot(c, u),
                            dot(c, v));

    return uv;
}

#endif
