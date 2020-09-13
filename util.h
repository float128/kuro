#ifndef KURO_UTIL_H
#define KURO_UTIL_H

#include "data-structures.h"
#include "vec4f.h"
#include "rgba.h"

using namespace kuro::util;
using namespace kuro::math;

/*the data returned from an intersection test*/
struct kuro::util::idata
{
    //position of the intersection
    vec4f p;

    //normal at the position of the intersection
	vec4f n;

    //if an intersection has occurred
	bool hit;

	//the distance with respect to the input ray to the intersection point
    kuro::util::real distance;

    //a pointer to the object intersected with
	kuro::object::base* hit_object;
};

/*holds the data used to traverse a scene*/
struct kuro::util::path
{
    //stores the position of the intersection
	vec4f p;

	//stores the normal at the position of the intersection
	vec4f n;

	 //stores the incoming direction from a BSDF
	vec4f in;

	//stores the outgoing direction from a BSDF
	vec4f out;

	//stores an attenuation value
	real atten;

	//stores the index of refraction for the current medium
	real ior;

	//stores the current colour; manipulated by BSDF's
	rgba colour;

    //stores if path should be terminated
    bool terminate;

    //stores if initial path for use a BRDF
    bool initial;

    //stores if background BRDF should be applied when no hit
    bool no_hit_brdf;
};

#endif // KURO_UTIL_IDATA
