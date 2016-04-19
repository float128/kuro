#ifndef KURO_BRDF_CPP
#define KURO_BRDF_CPP

#include <iostream>
#include "data-structures.h"
#include "brdf.h"
#include "util.h"
#include "brdf.h"

using namespace std;
using namespace kuro::util;
using namespace kuro::math;

void kuro::brdf::basic::reflect(path* pa)
{
    pa->colour.r *= colour.r;
    pa->colour.g *= colour.g;
    pa->colour.b *= colour.b;
    pa->colour.a *= colour.a;

    pa->terminate = true;
}

void kuro::brdf::ambient_occ::reflect(path* pa)
{
    pa->no_hit_brdf = false;

	pa->colour.r *= colour.r;
    pa->colour.g *= colour.g;
    pa->colour.b *= colour.b;
    pa->colour.a *= colour.a;

    pa->out = kuro::util::random::vec3f_random(pa->n);

    if(!pa->initial)pa->atten = pa->atten * 0.5;

    if(pa->atten < 0.05)pa->terminate = true;
}

void kuro::brdf::normal_diffuse::reflect(path* pa)
{
    pa->no_hit_brdf = false;
    pa->terminate = true;

	pa->colour.r *= colour.r;
    pa->colour.g *= colour.g;
    pa->colour.b *= colour.b;
    pa->colour.a *= colour.a;

    pa->atten *= dot(normalise(pa->n), normalise((-1.0) * pa->in));
}

void kuro::brdf::simple_reflect::reflect(path* pa)
{
    pa->colour.r *= colour.r;
    pa->colour.g *= colour.g;
    pa->colour.b *= colour.b;
    pa->colour.a *= colour.a;

	vec4f in = (-1.0) * normalise(pa->in);
	vec4f n = normalise(pa->n);
    vec4f h = dot(in, n) * n;
	vec4f d = h - in;

	pa->out = h + d;
}

void kuro::brdf::emission::reflect(path* pa)
{
    pa->colour.r *= colour.r;
    pa->colour.g *= colour.g;
    pa->colour.b *= colour.b;
    pa->colour.a *= colour.a;

    pa->colour = pa->colour * strength;

    pa->terminate = true;
}

void kuro::brdf::diffuse::reflect(path* pa)
{
    pa->colour.r *= colour.r;
    pa->colour.g *= colour.g;
    pa->colour.b *= colour.b;
    pa->colour.a *= colour.a;


    pa->out = kuro::util::random::vec3f_random(pa->n);

    pa->atten *= dot(normalise(pa->out), normalise(pa->n));
}

void kuro::brdf::mix::reflect(path* pa)
{
	if(brdf1 == NULL)
	{
		if(brdf2 != NULL)
		{
			brdf2->reflect(pa);
			return;
		}
	}
	else
	{
		if(brdf2 == NULL)
		{
			brdf1->reflect(pa);
			return;
		}
	}

	if(kuro::util::random::real_random() <= factor)
	{
		brdf1->reflect(pa);
	}
	else
	{
		brdf2->reflect(pa);
	}
}

#endif // KURO_BRDF
