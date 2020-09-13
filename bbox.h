#ifndef KURO_BBOX_H
#define KURO_BBOX_H

#include <vector>
#include "data-structures.h"

#include "vec4f.h"
#include "object.h"
#include "triangle.h"

class kuro::object::bbox
{
    private:
        kuro::util::idata intersect(kuro::math::ray4f ir, kuro::math::vec4f a, kuro::math::vec4f b, kuro::math::vec4f c);

    public:
        /*stores the low dimensions of the bounding box*/
        vec4f low;

        /*stores the high dimensions of the bounding box*/
        vec4f high;

        /*tests for an intersection with a bounding box*/
        bool intersect(kuro::math::ray4f ir);

		/*tests for an intersection with a bounding box and
		returns distance to intersection, with ray point outside bounding box*/
        bool intersect(kuro::math::ray4f ir, kuro::util::real* distance);

        /*tests for an intersection with a bounding box*/
        bool intersect(kuro::math::vec4f a, kuro::math::vec4f b);

        /*returns if the points lies in the Bounding Box*/
        bool in_bb(vec4f p);

        /*tests for an intersection between a triangle and a
        bounding box*/
        bool tri_intersect(vec4f a, vec4f b, vec4f c);

        /*tests for an intersection between a triangle and a
        bounding box, assuming triangle is in bounds for all axis
        not input into the function*/
        bool tri_intersect(vec4f a, vec4f b, vec4f c, kuro::util::uint axis);
};

#endif
