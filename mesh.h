#ifndef KURO_MESH_H
#define KURO_MESH_H

#include <vector>
#include "data-structures.h"
#include "vec4f.h"
#include "mat4f.h"
#include "object.h"
#include "bbox.h"
#include "triangle.h"
#include "octree.h"

/*an object that consists of triangles and vertices*/
class kuro::object::mesh: public kuro::object::base
{
    public:
        /*a dynamic array of vertices*/
        vector <vec4f> vertices;

		/*a dynamic array of vertex normals*/
		vector <vec4f> vnormals;

        /*a dynamic array of triangles*/
        vector <kuro::object::triangle> triangles;

        /*the bounding box of the mesh*/
        bbox bounding_box;

        mat4f transform_matrix;
		mat4f inv_transform_matrix;

        bool smooth_shading;

        void apply_transformation();

    public:
        /*returns the intersection of a ray and a mesh*/
        kuro::util::idata intersect(kuro::math::ray4f ir);

        /*translates the object the object by delta*/
        void translate(vec4f delta);

        /*rotates the object about the origin and axis*/
        void rotate(vec4f axis, kuro::util::real theta);

        /*scales the object about the origin for each component
        by the given scale factor*/
        void scale(vec4f scale_factor);

        /*imports an wavefront object file format into mesh data*/
        bool import(std::string filename);

        /*calculates the bounding box of the mesh from vertices*/
        bbox calculateBB();
};

/*returns the uv coordinates of the point p with respect to the
vectors u and v from the given origin*/
kuro::math::vec2f uv_coords(kuro::math::vec4f origin,
                            kuro::math::vec4f u,
                            kuro::math::vec4f v,
                            kuro::math::vec4f p);

#endif
