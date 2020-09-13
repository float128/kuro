#ifndef KURO_TRIANGLE_H
#define KURO_TRIANGLE_H

#include <vector>
#include "data-structures.h"
#include "vec4f.h"
#include "object.h"

/*stores the indices in an array of vertices that
represent a triangle*/
struct kuro::object::triangle
{
    /*the indices in an array of points to the points of the triangle*/
    kuro::util::uint point[3];

	/*the indices in an array to the vertex normals to the vertex normals of the triangle*/
	kuro::util::uint vnormal[3];

    /*constructor*/
    triangle()
    {

    }

    /*constructor for triangle*/
    triangle(kuro::util::uint a, kuro::util::uint b, kuro::util::uint c)
    {
        point[0] = a;
        point[1] = b;
        point[2] = c;
    }

    /*returns data relating to the intersection of a 3d triangle with a ray*/
    kuro::util::idata intersect(kuro::math::ray4f ir, vec4f* vertices);

	/*returns data relating to the intersection of a 3d triangle with a ray*/
    kuro::util::idata intersect(kuro::math::ray4f ir, vec4f* vertices, vec4f* vnormals);
};

kuro::math::vec2f uv_coords(kuro::math::vec4f origin,
                            kuro::math::vec4f u,
                            kuro::math::vec4f v,
                            kuro::math::vec4f p);

#endif
