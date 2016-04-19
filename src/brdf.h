#ifndef KURO_BRDF_H
#define KURO_BRDF_H

#include "data-structures.h"

#include "util.h"
#include "rgba.h"

using namespace kuro::util;
using namespace kuro::math;

class kuro::brdf::brdf
{
    public:
        virtual void reflect(path* pa)=0;
};

class kuro::brdf::basic: public kuro::brdf::brdf
{
    public:
        rgba colour;
        void reflect(path* pa);
};

class kuro::brdf::ambient_occ: public kuro::brdf::brdf
{
    public:
		rgba colour;
        void reflect(path* pa);
};

class kuro::brdf::normal_diffuse: public kuro::brdf::brdf
{
    public:
		rgba colour;
        void reflect(path* pa);
};

class kuro::brdf::simple_reflect: public kuro::brdf::brdf
{
    public:
        rgba colour;
        void reflect(path* pa);
};

class kuro::brdf::emission: public kuro::brdf::brdf
{
    public:
        rgba colour;
        kuro::util::real strength;

        void reflect(path* pa);
};

class kuro::brdf::diffuse: public kuro::brdf::brdf
{
    public:
        rgba colour;

        void reflect(path* pa);
};

class kuro::brdf::mix: public kuro::brdf::brdf
{
	public:
		kuro::util::real factor;
		brdf* brdf1;
		brdf* brdf2;

		void reflect(path* pa);
};

#endif // KURO_BRDF
