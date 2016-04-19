#ifndef KURO_OBJECT_H
#define KURO_OBJECT_H

#include "data-structures.h"
#include "vec2f.h"
#include "vec4f.h"
#include "brdf.h"

using namespace kuro::util;
using namespace kuro::math;

/*the base class for objects*/
class kuro::object::base
{
    public:
        //returns the intersection data for a ray and an object
        virtual idata intersect(ray4f r) = 0;

        //translates the object the object by delta
        virtual void translate(vec4f delta){};

        //rotates the object about the origin and axis
        virtual void rotate(vec4f axis, kuro::util::real theta){};

        /*scales the object about the origin for each component
        by the given scale factor*/
        virtual void scale(vec4f scale_factor){};

        //the BRDF for an object
        brdf::brdf* object_brdf;

        //stores the next object for a linked list
        kuro::object::base* next;
};

/*a sphere object that inherits the base object class*/
class kuro::object::sphere: public kuro::object::base
{
    public:
        //position of sphere
        vec4f p;

        //radius of the sphere
        real r;

        //returns intersection data for a ray and sphere
        idata intersect(ray4f ir);
};

/*a disk object that inherits the base object class*/
class kuro::object::disk: public kuro::object::base
{
    public:
        //position of disk
        vec4f p;

        //normal of disk
        vec4f n;

        //radius of disk
        real r;

        //returns intersection data for a ray and disk
        idata intersect(ray4f ir);
};

/*a line object that inherits the base object class*/
class kuro::object::line: public kuro::object::base
{
    public:
        //starting position of line
        vec4f p;

        //direction of line
        vec4f d;

        //radius of line
        real r;

        //returns intersection data for a ray and a line
        idata intersect(ray4f ir);
};

/*
solves 2 linear equations
ax + by = u
cx + dy = v
*/
vec2f linear_solve(real a, real b, real c, real d, real u, real v);

#endif
