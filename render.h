#ifndef KURO_RENDER_H
#define KURO_RENDER_H

#include <ctime>
#include "data-structures.h"

#include "ray4f.h"
#include "bmp.h"
#include "brdf.h"
#include "scene.h"

using namespace kuro::util;
using namespace kuro::math;

namespace kuro
{
    namespace render
    {
		/*traces a single ray throughout a scene*/
        rgba trace(ray4f ir, kuro::scene::scene* scn);

		/*renders a block within an image*/
		void render_block(uint start_x, uint start_y, uint end_x, uint end_y, kuro::image::base* img, kuro::scene::scene* scn);

        void print_progress(int x, int y, int w, int h, int count, clock_t t0);
    }
}
#endif
