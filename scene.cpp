#ifndef KURO_SCENE_CPP
#define KURO_SCENE_CPP

#include "data-structures.h"
#include "util.h"
#include "scene.h"

using namespace kuro;
using namespace kuro::util;
using namespace kuro::math;

/*returns the intersection with
the scene (collection of object)*/
idata scene::scene::intersect(ray4f ir)
{
    idata data;
    idata tmp_data;

    data.hit = false;

    for(object::base* obj = object;obj!=NULL;obj=obj->next)
    {
        tmp_data = obj->intersect(ir);

        if(!tmp_data.hit)continue;

        if((len(tmp_data.p - ir.p)<len(data.p - ir.p))||(!data.hit))data = tmp_data;
    }

    if(data.hit)data.n = flip_normal(data.n, ir.d);

    return data;
}

vec4f flip_normal(vec4f n, vec4f d)
{
    if(dot(normalise(n), normalise(d))<=0)return n;
    return (-1.0) * n;
}

#endif
