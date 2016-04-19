#ifndef KURO_OCTREE_H
#define KURO_OCTREE_H

#include <vector>
#include "data-structures.h"
#include "vec4f.h"
#include "object.h"
#include "bbox.h"

class kuro::object::octree
{
    public:
        typedef vector <vec4f> vertices;

        typedef vector <kuro::object::triangle> triangles;

    public:
        /*calculates the bounding box of a given set of vertices*/
        void calculateBB(vertices* ivertices);

    public:
        /*indicates if node has child nodes*/
        bool leaf;

        /*points to all the child nodes of the node*/
        octree* child[2][2][2];

        /*stores all the indices of all the triangles held by this node*/
        vector <kuro::util::uint> faces;

        /*stores the bounding box of the node*/
        bbox bounding_box;

        /*stores the depth of the node*/
        int depth;

        /*returns a pointer to an octree node containing triangles or
        a NULL pointer if there are no nodes etc.*/
        //octree* intersect(kuro::math::ray4f ir);

        /*partitions an octree node into child nodes etc.*/
        void partition(vertices* ivertices,
                       triangles* itriangles,
                       octree* ioctree, int max_depth);
};

#endif
