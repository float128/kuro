#ifndef KURO_OCTREE_CPP
#define KURO_OCTREE_CPP

#include <vector>
#include <math.h>

#include "data-structures.h"
#include "vec4f.h"
#include "object.h"
#include "octree.h"

void kuro::object::octree::calculateBB(vertices* ivertices)
{
    bounding_box.low = vec4f(INFINITY, INFINITY, INFINITY);
    bounding_box.high = vec4f(-INFINITY, -INFINITY, -INFINITY);

    vec4f vertex;

    for(int i = 0;i<ivertices->size();i++)
    {
        vertex = (*ivertices)[i];

        if(vertex.x < bounding_box.low.x) bounding_box.low.x = vertex.x;
        if(vertex.y < bounding_box.low.y) bounding_box.low.y = vertex.y;
        if(vertex.z < bounding_box.low.z) bounding_box.low.z = vertex.z;

        if(vertex.x > bounding_box.high.x) bounding_box.high.x = vertex.x;
        if(vertex.y > bounding_box.high.y) bounding_box.high.y = vertex.y;
        if(vertex.z > bounding_box.high.z) bounding_box.high.z = vertex.z;
    }
}

void kuro::object::octree::partition(vertices* ivertices,
                                     triangles* itriangles,
                                     octree* ioctree, int max_depth)
{
    for(int i = 0;i<=1;i++)
    {
        for(int j = 0;j<=1;j++)
        {
            for(int k = 0;k<=1;k++)
            {
                child[i][j][k] = NULL;
            }
        }
    }

    if(ioctree == NULL)
    {
        depth = 0;

        calculateBB(ivertices);

        for(int i = 0;i<itriangles->size();i++)
        {
            faces.push_back(i);
        }

        leaf = false;
        if(itriangles->size() <= 16)leaf = true;

        if(leaf)return;
    }

    //cout<< "starting... depth= " << depth <<endl;

    if(ioctree != NULL)
    {
        for(int i = 0;i<ioctree->faces.size();i++)
        {
            //cout<< i << " of "  << ioctree->faces.size() <<endl;

            vec4f a, b, c;
            a  = (*ivertices)[(*itriangles)[ioctree->faces[i]].point[0]];
            b = (*ivertices)[(*itriangles)[ioctree->faces[i]].point[1]];
            c = (*ivertices)[(*itriangles)[ioctree->faces[i]].point[2]];

            if(bounding_box.tri_intersect(a, b, c))
            {
                faces.push_back(ioctree->faces[i]);
            }
        }
    }

    //cout<< "faces: " << faces.size() <<endl;

    leaf = false;
    if(faces.size() <= 16)leaf = true;
    if(depth >= max_depth)leaf = true;

    if(leaf)return;

    kuro::math::vec4f delta;
    delta = bounding_box.high - bounding_box.low;
    delta = delta * 0.5;

    for(int i = 0;i<=1;i++)
    {
        for(int j = 0;j<=1;j++)
        {
            for(int k = 0;k<=1;k++)
            {
                bbox bb;
                bb.low = bounding_box.low;

                if(i == 0)
                {
                    bb.low.x += delta.x;
                }

                if(j == 0)
                {
                    bb.low.y += delta.y;
                }

                if(k == 0)
                {
                    bb.low.z += delta.z;
                }

                bb.high = bb.low + delta;

                child[i][j][k] = new octree;
                child[i][j][k]->bounding_box = bb;
                child[i][j][k]->depth = depth+1;

                child[i][j][k]->partition(ivertices, itriangles, this, max_depth);
            }
        }
    }


    if(!leaf)faces.clear();
}


#endif
