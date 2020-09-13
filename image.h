#ifndef KURO_IMAGE_H
#define KURO_IMAGE_H

#include <string>
#include <vector>
#include "data-structures.h"

using namespace std;
using namespace kuro;
using namespace kuro::util;
using namespace kuro::math;

class kuro::image::base
{
    /*
    location (x, y) on the image s such that
    (0, 0) is at the top left.

    (width-1, height-1) is at the bottom right.

    x is the column number starting from row 0.
    y is the row number starting from column 0.
    */
    public:
        /*gets the value of the pixel at location (x, y) in pixels*/
        virtual rgba get(uint x, uint y) = 0;

        /*sets the value of the pixel at location (x, y) in pixels*/
        virtual void set(uint x, uint y, rgba) = 0;

        /*gets the width of the image in pixels*/
        virtual uint width() = 0;

        /*gets the height of the image in pixels*/
        virtual uint height() = 0;

        /*resizes the dimensions of the image*/
        virtual void resize(uint w, uint h) = 0;

        /*saves the image at location addr*/
        virtual void save(string addr){}

		/*applies gamma correction to the image*/
		void gamma_correction(kuro::util::real correction_value);
};

class kuro::image::image: public kuro::image::base
{
    private:
        typedef vector <rgba> column;
        vector <column> columns;

    public:
        /*gets the value of the pixel at location (x, y) in pixels*/
        rgba get(uint x, uint y);

        /*sets the value of the pixel at location (x, y) in pixels*/
        void set(uint x, uint y, rgba colour);

        /*gets the width of the image in pixels*/
        uint width();

        /*gets the height of the image in pixels*/
        uint height();

        /*resizes the dimensions of the image*/
        void resize(uint w, uint h);
};

#endif // KURO_IMAGE_H
