#ifndef KURO_IMAGE_BMP_CPP
#define KURO_IMAGE_BMP_CPP

#include <string>
#include "data-structures.h"
#include "bitmap_image.hpp"
#include "rgba.h"
#include "image.h"
#include "bmp.h"

using namespace std;
using namespace kuro;
using namespace kuro::util;
using namespace kuro::math;

void image::bmp::save(string addr)
{
	bitmap_image img(width(), height());
	img.set_all_channels(0, 0, 0);

    rgba p;
	char c[3];

	for(uint y = 0;y<height();y++)
	{
		for(uint x = 0;x<width();x++)
		{
            p = get(x, y);

			if(p.r > 1.0)p.r = 1.0;
			if(p.g > 1.0)p.g = 1.0;
			if(p.b > 1.0)p.b = 1.0;

			if(p.r < 0.0)p.r = 0.0;
			if(p.g < 0.0)p.g = 0.0;
			if(p.b < 0.0)p.b = 0.0;

            c[0] = p.r * 255;
            c[1] = p.g * 255;
            c[2] = p.b * 255;

			img.set_pixel(x, y,c[0], c[1], c[2]);
		}
	}

	img.save_image(addr.c_str());
}

#endif // KURO_IMAGE_H
