#define _SCL_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <regex>
#include <stdlib.h>
#include "util.h"
#include "util.cpp"
#include "brdf.h"
#include "brdf.cpp"
#include "object.h"
#include "object.cpp"
#include "bmp.h"
#include "bmp.cpp"
#include "camera.h"
#include "camera.cpp"
#include "scene.h"
#include "scene.cpp"
#include "render.h"
#include "render.cpp"

using namespace std;
using namespace kuro;
using namespace kuro::math;
using namespace kuro::util;

int main()
{
    int w=512, h=512, samples=1;

    camera::camera cam;
    cam.fov_x = 90.0;
    cam.fov_y = 90.0;
    cam.p = vec4f();

    image::bmp img;
    img.resize(w, h);

    brdf::basic brdf_black;
    brdf::basic brdf_red;

    brdf_black.colour = rgba(0.0, 0.0, 0.0);
    brdf_red.colour = rgba(1.0, 0.0, 0.0);

    object::sphere s1;
    s1.p = vec4f(0.0, 0.0, 10.0);
    s1.r = 1.0;
    s1.object_brdf = &brdf_red;
    s1.next = NULL;

    scene::scene scn;
    scn.object = &s1;
	scn.camera = &cam;
    scn.background_brdf = &brdf_black;
	scn.settings.samples = samples;

    render::render_block(0, 0, 511, 511, &img, &scn);
    img.save("camera-output.bmp");


	system("pause");
	return 0;
}
