#ifndef KURO_RENDER_CPP
#define KURO_RENDER_CPP

#include <ctime>
#include "data-structures.h"
#include "ray4f.h"
#include "scene.h"
#include "render.h"

using namespace kuro::util;
using namespace kuro::math;

rgba kuro::render::trace(ray4f ir, kuro::scene::scene* scn)
{
    util::path pa;
    util::idata data;

	/*initiates the first ray of the path from the input ray*/
    pa.colour = rgba(1.0, 1.0, 1.0, 1.0);
    pa.p = ir.p;
    pa.in = ir.d;
    pa.atten = 1.0;
    pa.ior = 1.0;
    pa.terminate = false;
    pa.initial = true;
    pa.no_hit_brdf = true;

	/*traces the path taken until no intersection
	occurs*/
    while(true)
    {
        //bias to prevent self intersection
        pa.p = pa.p + (pa.in * 0.001);

        data = scn->intersect(ray4f(pa.p, pa.in));

		/*exits the loop when no intersection with the scene occurs*/
        if(!data.hit)
        {
            if(pa.no_hit_brdf)scn->background_brdf->reflect(&pa);
            break;
        }

        pa.p = data.p;
		pa.n = data.n;
        data.hit_object->object_brdf->reflect(&pa);

        if(pa.terminate)
        {
            break;
        }

        if(pa.atten < 0.001)
        {
            break;
        }

        pa.in = pa.out;
        pa.initial = false;
    }

    return pa.atten * pa.colour;
}

void kuro::render::render_block(uint start_x, uint start_y, uint end_x, uint end_y, kuro::image::base* img, kuro::scene::scene* scn)
{
    int count = 0;

    clock_t t0 = clock();

	/*renders each pixel belonging to the input region
	of the image*/
	for(int i = start_x;i<=end_x;i++)
	{
		for(int j = start_y;j<=end_y;j++)
		{
            rgba col = rgba(0.0, 0.0, 0.0, 0.0);

            for(int k = 0;k<scn->settings.samples;k++)
            {
                vec2f p((((real) i)/img->width()), (((real) j)/img->height()));
				
				/*samples random points within each pixel to reduce aliasing*/
                p.x += util::random::real_random() * (((real) 1.0)/img->width());
                p.y += util::random::real_random() * (((real) 1.0)/img->height());
                p.y = 1.0 - p.y;

                p = (2.0*p) - vec2f(1.0, 1.0);

				/*gets the ray from the camera for a particular
				point of the sensor of the camera*/
                ray4f r = scn->camera->get_ray(p);

				/*traces the ray throughout the scene*/
                col = col + kuro::render::trace(r, scn);
            }
			/*averages the all the samples taken*/
            col = col * (1.0/scn->settings.samples);

			/*applies the colour to the respective pixel*/
            img->set(i, j, col);

			/*prints the progress of the rendering*/
            print_progress(i - start_x, j - start_y, end_x - start_x, end_y - start_y, count, t0);
            count++;
		}
	}
}

void kuro::render::print_progress(int x, int y, int w, int h, int count, clock_t t0)
{
    kuro::util::real rw = w;
    kuro::util::real rh = h;
    kuro::util::real rdim = rw * rh;

    int percent_1 = rdim*0.01;

    if(percent_1 == 0)return;
	/*outputs the % of the rendering completed
	when increased by 1%*/
    if(count % percent_1 == 0)
    {
        std::cout<< "render % completion: " << (int) ((((kuro::util::real) count)/(rdim)) * 100);
        std::cout<< "\t time elapsed: "<< ((float) (clock()-t0) )/CLOCKS_PER_SEC<< " secs";
        std::cout<<std::endl;
    }
}

#endif
