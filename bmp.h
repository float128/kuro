#ifndef KURO_IMAGE_BMP_H
#define KURO_IMAGE_BMP_H

#include <string>
#include "data-structures.h"
#include "image.h"

using namespace std;
using namespace kuro;
using namespace kuro::util;
using namespace kuro::math;

class kuro::image::bmp: public kuro::image::image
{
    public:
		/*saves the image*/
        void save(string addr);
};

#endif // KURO_IMAGE_H
