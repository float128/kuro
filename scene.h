#ifndef KURO_SCENE_H
#define KURO_SCENE_H

#include "data-structures.h"

#include "util.h"
#include "object.h"
#include "camera.h"

using namespace kuro::util;
using namespace kuro::math;

/*stores the settings for rendering an image*/
struct kuro::scene::render_settings
{
    uint samples, threads, width, height;
    string name;
};

/*stores all scene data necessary for
rendering an image*/
class kuro::scene::scene
{
    public:
        /*stores the settings of the scene*/
        render_settings settings;

        /*stores the background BRDF when
        no intersection is found*/
        brdf::brdf* background_brdf;

		/*stores the camera used to render the scene*/
		camera::base* camera;

        /*stores the first object in a
        linked list of objects*/
        object::base* object;

        /*returns the intersection with
        the scene (collection of object)*/
        idata intersect(ray4f ir);
};

vec4f flip_normal(vec4f n, vec4f d);

#endif
