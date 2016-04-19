#ifndef KURO_IMAGE_CPP
#define KURO_IMAGE_CPP

#include <string>
#include <vector>
#include <math.h>

#include "data-structures.h"
#include "image.h"
#include "rgba.h"

using namespace std;
using namespace kuro;
using namespace kuro::util;
using namespace kuro::math;


rgba image::image::get(uint x, uint y)
{
    return columns[x][y];
}

void image::image::set(uint x, uint y, rgba colour)
{
    columns[x][y] = colour;
}

uint image::image::width()
{
    return columns.size();
}

uint image::image::height()
{
    if(columns.size()>0)return columns[0].size();
    return 0;
}

void image::image::resize(uint w, uint h)
{
    if(height() > 0)
    {
        for(int i = 0;i<width();i++)
        {
            columns[i].clear();
        }
    }

    columns.resize(w);

    for(int i = 0;i<w;i++)
    {
        columns[i].resize(h);
    }
}

void kuro::image::base::gamma_correction(kuro::util::real correction_value)
{
	rgba value;


	for(int j = 0;j<height();j++)
	{
		for(int i = 0;i<width();i++)
		{
			value = get(i, j);

			value.r = pow(value.r, correction_value);
			value.g = pow(value.g, correction_value);
			value.b = pow(value.b, correction_value);
			value.a = pow(value.a, correction_value);

			set(i, j, value);
		}
	}
}


#endif
