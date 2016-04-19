#ifndef KURO_BBOX_CPP
#define KURO_BBOX_CPP

#include <vector>
#include "data-structures.h"
#include "vec2f.h"
#include "vec4f.h"
#include "ray4f.h"
#include "util.h"
#include "object.h"
#include "bbox.h"

kuro::util::idata kuro::object::bbox::intersect(kuro::math::ray4f ir, kuro::math::vec4f a, kuro::math::vec4f b, kuro::math::vec4f c)
{
    kuro::util::idata data;
    data.hit_object = NULL;
    data.hit = false;

    kuro::math::vec4f origin = a;

    kuro::math::vec4f u = b - a;
    kuro::math::vec4f v = c - a;

    kuro::math::vec4f n = cross(u, v);

    kuro::util::real x = dot(origin - ir.p, n)/dot(ir.d, n);

    data.distance = x;

    data.p = ir.p + (ir.d * x);
    data.n = n;


    kuro::math::vec2f uv = uv_coords(origin, u, v, data.p);

    if((uv.x >= 0)&&(uv.y >= 0)&&(uv.x + uv.y <= 1.0))
    {
            data.hit = true;
    }

    return data;
}

bool kuro::object::bbox::intersect(kuro::math::ray4f ir)
{
    kuro::util::real tmin, tmax, tymin, tymax, tzmin, tzmax;

    vec4f* bounds[2];
    int sign[3];
    /*
    bbox bb;
    bb.low = low;
    bb.high = high;

	vec4f delta = high - low;
	delta = delta * 0.0f;

	bb.high = bb.high + delta;
	bb.low = bb.low - delta;

    bounds[0] = &bb.low;
    bounds[1] = &bb.high;
    */

    bounds[0] = &low;
    bounds[1] = &high;

    sign[0] = (ir.d.x < 0.0);
    sign[1] = (ir.d.y < 0.0);
    sign[2] = (ir.d.z < 0.0);

    tmin = (bounds[sign[0]]->x - ir.p.x) / ir.d.x;
    tmax = (bounds[1-sign[0]]->x - ir.p.x) / ir.d.x;
    tymin = (bounds[sign[1]]->y - ir.p.y) / ir.d.y;
    tymax = (bounds[1-sign[1]]->y - ir.p.y) / ir.d.y;

    if ( (tmin > tymax) || (tymin > tmax) ) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    tzmin = (bounds[sign[2]]->z - ir.p.z) / ir.d.z;
    tzmax = (bounds[1-sign[2]]->z - ir.p.z) / ir.d.z;

    if ( (tmin > tzmax) || (tzmin > tmax) ) return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    return ((tmin >= 0.0)||(tmin*tmax <= 0.0));
}

bool kuro::object::bbox::intersect(kuro::math::ray4f ir, kuro::util::real* distance)
{
	kuro::util::real tmin, tmax, tymin, tymax, tzmin, tzmax;

    vec4f* bounds[2];
    int sign[3];
    /*
    bbox bb;
    bb.low = low;
    bb.high = high;

	vec4f delta = high - low;
	delta = delta * 0.0f;

	bb.high = bb.high + delta;
	bb.low = bb.low - delta;

    bounds[0] = &bb.low;
    bounds[1] = &bb.high;
    */

    bounds[0] = &low;
    bounds[1] = &high;

    sign[0] = (ir.d.x < 0.0);
    sign[1] = (ir.d.y < 0.0);
    sign[2] = (ir.d.z < 0.0);

    tmin = (bounds[sign[0]]->x - ir.p.x) / ir.d.x;
    tmax = (bounds[1-sign[0]]->x - ir.p.x) / ir.d.x;
    tymin = (bounds[sign[1]]->y - ir.p.y) / ir.d.y;
    tymax = (bounds[1-sign[1]]->y - ir.p.y) / ir.d.y;

    if ( (tmin > tymax) || (tymin > tmax) ) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    tzmin = (bounds[sign[2]]->z - ir.p.z) / ir.d.z;
    tzmax = (bounds[1-sign[2]]->z - ir.p.z) / ir.d.z;

    if ( (tmin > tzmax) || (tzmin > tmax) ) return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    bool hit = (tmin >= 0.0);

	if(hit)*distance = tmax;

	return hit;
}

bool kuro::object::bbox::intersect(kuro::math::vec4f a, kuro::math::vec4f b)
{
    kuro::math::ray4f ir(a, b - a);

    kuro::util::real tmin, tmax, tymin, tymax, tzmin, tzmax;

    vec4f* bounds[2];
    int sign[3];

    bounds[0] = &low;
    bounds[1] = &high;

    sign[0] = (ir.d.x < 0.0);
    sign[1] = (ir.d.y < 0.0);
    sign[2] = (ir.d.z < 0.0);

    tmin = (bounds[sign[0]]->x - ir.p.x) / ir.d.x;
    tmax = (bounds[1-sign[0]]->x - ir.p.x) / ir.d.x;
    tymin = (bounds[sign[1]]->y - ir.p.y) / ir.d.y;
    tymax = (bounds[1-sign[1]]->y - ir.p.y) / ir.d.y;

    if ( (tmin > tymax) || (tymin > tmax) ) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    tzmin = (bounds[sign[2]]->z - ir.p.z) / ir.d.z;
    tzmax = (bounds[1-sign[2]]->z - ir.p.z) / ir.d.z;

    if ( (tmin > tzmax) || (tzmin > tmax) ) return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    return (tmin > 0.0) && (tmax < 1.0);
}

bool kuro::object::bbox::in_bb(vec4f p)
{
    return  (p.x >= low.x)&&(p.x <= high.x)&&
            (p.y >= low.y)&&(p.y <= high.y)&&
            (p.z >= low.z)&&(p.z <= high.z);
}

bool kuro::object::bbox::tri_intersect(vec4f a, vec4f b, vec4f c)
{
    bbox bb;

	vec4f delta = high - low;
	delta = delta * 0.001;

	bb.high = high + delta;
	bb.low = low - delta;

    if(in_bb(a))return true;
    if(in_bb(b))return true;
    if(in_bb(c))return true;

    if(intersect(a, b))return true;
    if(intersect(a, c))return true;
    if(intersect(b, c))return true;

    vec4f verts[2][2][2];
    vec4f edge[2];
    vec4f tri_verts[3] = {a, b, c};
    kuro::util::idata data;

    triangle t(0, 1, 2);

    for(int i = 0;i<=1;i++)
    {
        for(int j = 0;j<=1;j++)
        {
            for(int k = 0;k<=1;k++)
            {
                if(i == 0)
                {
                    verts[i][j][k].x = bb.low.x;
                }
                else
                {
                    verts[i][j][k].x = bb.high.x;
                }

                if(j == 0)
                {
                    verts[i][j][k].y = bb.low.y;
                }
                else
                {
                    verts[i][j][k].y = bb.high.y;
                }

                if(k == 0)
                {
                    verts[i][j][k].z = bb.low.z;
                }
                else
                {
                    verts[i][j][k].z = bb.high.z;
                }
            }
        }
    }

    vec4f square[2][4];

    int count = 0;

    for(int i = 0;i<2;i++)
    {
        for(int j = 0;j<2;j++)
        {
            square[0][count] = verts[0][i][j];
            square[1][count] = verts[1][i][j];
            count++;
        }
    }

    vec4f tmp;
    tmp = square[0][3];
    square[0][3] = square[0][2];
    square[0][2] = tmp;

    tmp = square[1][3];
    square[1][3] = square[1][2];
    square[1][2] = tmp;

    for(int k = 0;k<3;k++)
    {
        for(int i = 0;i<4;i++)
        {
            int j = i+1;
            if(i == 3)j = 0;

            int l[2] = {k, k};

            if(k == 2)
            {
                l[0] = 0;
                l[1] = 1;
                j = i;
            }

            edge[0] = square[l[0]][i];
            edge[1] = square[l[1]][j];

            data = t.intersect(ray4f(edge[0], edge[1] - edge[0]), tri_verts);

            if(data.hit)
            {
                if(len(edge[0] - data.p) <= len(edge[0] - edge[1]))return true;
            }

        }
    }


    return false;
}

bool kuro::object::bbox::tri_intersect(vec4f a, vec4f b, vec4f c, kuro::util::uint axis)
{
    bbox bb;

	vec4f delta = high - low;
	delta = delta * 0.001;

	bb.high = high + delta;
	bb.low = low - delta;

    if(bb.in_bb(a))return true;
    if(bb.in_bb(b))return true;
    if(bb.in_bb(c))return true;

    vec4f verts[2][2][2];
    kuro::util::idata data;

    for(int i = 0;i<=1;i++)
    {
        for(int j = 0;j<=1;j++)
        {
            for(int k = 0;k<=1;k++)
            {
                if(i == 0)
                {
                    verts[i][j][k].x = bb.low.x;
                }
                else
                {
                    verts[i][j][k].x = bb.high.x;
                }

                if(j == 0)
                {
                    verts[i][j][k].y = bb.low.y;
                }
                else
                {
                    verts[i][j][k].y = bb.high.y;
                }

                if(k == 0)
                {
                    verts[i][j][k].z = bb.low.z;
                }
                else
                {
                    verts[i][j][k].z = bb.high.z;
                }
            }
        }
    }

    bool upper_bound = false;
    bool lower_bound = false;
    bool hit = false;

    vec4f n;

    if(axis == 0)
    {
        n = vec4f(1.0);
    }
    else if (axis == 1)
    {
        n = vec4f(0.0, 1.0);
    }
    else if (axis == 2)
    {
        n = vec4f(0.0, 0.0, 1.0);
    }

    for(int i = 0;i<=1;i++)
    {
        for(int j = 0;j<=1;j++)
        {
            for(int k = 0;k<=1;k++)
            {
                data = intersect(ray4f(verts[i][j][k], n), a, b, c);

                //cout<< data.distance<<endl;

                if(data.hit)
                {
                    hit = true;
                }

                if(data.distance > 0.0)
                {
                    upper_bound = true;
                }
                else
                {
                    lower_bound = true;
                }
            }

        }
    }

    return (upper_bound&&lower_bound&&hit);
}

#endif
