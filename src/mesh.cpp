#ifndef KURO_MESH_CPP
#define KURO_MESH_CPP

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#include <vector>
#include <stdio.h>
#include <math.h>
#include <fstream>

#include "data-structures.h"
#include "vec4f.h"
#include "object.h"
#include "mesh.h"
#include "ray4f.h"
#include "util.h"
#include "vec2f.h"
#include "vec4f.h"
#include "mat4f.h"
#include "object.h"

kuro::util::idata kuro::object::mesh::intersect(kuro::math::ray4f ir)
{
    idata data;
    idata tmp_data;

    data.hit = false;

	/*tests for an intersection with every triangle
	belonging to the mesh*/
    for(kuro::util::uint i = 0;i < triangles.size();i++)
    {
        tmp_data = triangles[i].intersect(ir, &vertices[0]);

        if(!tmp_data.hit)continue;

		/*finds the closest intersection to the position
		of the ray*/
        if((len(tmp_data.p - ir.p)<len(data.p - ir.p))||(!data.hit))data = tmp_data;
    }

    data.hit_object = this;

    return data;
}

void kuro::object::mesh::translate(vec4f delta)
{
	/*updates the transformation matrix to include
	the translation*/
    mat4f M = math::transform::translate(delta);
	mat4f inv_M = math::transform::translate(delta * -1.0);

    transform_matrix = transform_matrix * M;
	inv_transform_matrix = inv_M * inv_transform_matrix;
}

void kuro::object::mesh::rotate(vec4f i_axis, kuro::util::real theta)
{
	/*updates the transformation matrix to include
	the rotation*/
    mat4f M = kuro::math::transform::rotate(i_axis, theta);
	mat4f inv_M = kuro::math::transform::rotate(i_axis, -theta);

	transform_matrix = transform_matrix * M;
	inv_transform_matrix = inv_M * inv_transform_matrix;
}

void kuro::object::mesh::scale(vec4f scale_factor)
{
    mat4f M = math::transform::scale(scale_factor);

	scale_factor.x = 1.0/scale_factor.x;
	scale_factor.y = 1.0/scale_factor.y;
	scale_factor.z = 1.0/scale_factor.z;
	
	/*updates the transformation matrix to include
	the scaling of the mesh*/
	mat4f inv_M = math::transform::scale(scale_factor);

	transform_matrix = transform_matrix * M;
	inv_transform_matrix = inv_M * inv_transform_matrix;
}

void kuro::object::mesh::apply_transformation()
{
	/*applies the transformation matrix to every vertex
	belonging to the mesh*/
    for(kuro::util::real i = 0;i<vertices.size();i++)
    {
        vertices[i].w = 1.0;
        vertices[i] = transform_matrix * vertices[i];
        vertices[i].w = 0.0;
    }
}

bool kuro::object::mesh::import(string filename)
{
    ifstream file(filename.c_str());
    string line = "";

    bool include_vn = false;
    bool include_vt = false;

    kuro::math::vec4f v;
    kuro::object::triangle tri;

	/*reads the file line by line*/
    while(file)
    {
        getline(file, line);

        char buffer[32] = {0, 0, 0, 0};
        sscanf(line.c_str(), "%s", buffer);
        string type = buffer;

		/*checks if the first charactor of the line is 'v'*/
        if(type == "v")
        {
            float number[3];
            sscanf(line.c_str(), "%s %f %f %f", buffer, &number[0], &number[1], &number[2]);

            v.x = number[0];
            v.y = number[1];
            v.z = number[2];

            vertices.push_back(v);
        }

		/*checks if the first charactor of the line is 'vt'*/
        if(type == "vt")
        {
            float number[2];
            sscanf(line.c_str(), "%s %f %f", buffer, &number[0], &number[1]);

            include_vt = true;
        }

		/*checks if the first charactor of the line is 'vn'*/
        if(type == "vn")
        {
            float number[3];
            sscanf(line.c_str(), "%s %f %f %f", buffer, &number[0], &number[1], &number[2]);

			v.x = number[0];
            v.y = number[1];
            v.z = number[2];

			vnormals.push_back(v);

            include_vn = true;
        }

		/*checks if the first charactor of the line is 'f'*/
        if(type == "f")
        {
            int vertex[3] = {0, 0, 0};
            int vertex_normal[3] = {0, 0, 0};
            int vertex_texture[3] = {0, 0, 0};

            if(include_vt)
            {
                if(include_vn)
                {
                    sscanf(line.c_str(), "%s %d/%d/%d %d/%d/%d %d/%d/%d", buffer,
                                                                &vertex[0],
                                                                &vertex_texture[0],
                                                                &vertex_normal[0],
                                                                &vertex[1],
                                                                &vertex_texture[1],
                                                                &vertex_normal[1],
                                                                &vertex[2],
                                                                &vertex_texture[2],
                                                                &vertex_normal[2]);
                }
                else
                {
                    sscanf(line.c_str(), "%s %d/%d %d/%d %d/%d", buffer,
                                                                &vertex[0],
                                                                &vertex_texture[0],
                                                                &vertex[1],
                                                                &vertex_texture[1],
                                                                &vertex[2],
                                                                &vertex_texture[2]);
                }
            }
            else
            {
                if(include_vn)
                {
                    sscanf(line.c_str(), "%s %d//%d %d//%d %d//%d", buffer,
                                                                &vertex[0],
                                                                &vertex_normal[0],
                                                                &vertex[1],
                                                                &vertex_normal[1],
                                                                &vertex[2],
                                                                &vertex_normal[2]);
                }
                else
                {
                    sscanf(line.c_str(), "%s %d %d %d", buffer,
                                                                &vertex[0],
                                                                &vertex[1],
                                                                &vertex[2]);
                }
            }

			/*subtracts one from the indices for the vertices,
			vertex normals, vertex textures etc. for the faces*/
            vertex[0]--;
            vertex[1]--;
            vertex[2]--;

            vertex_normal[0]--;
            vertex_normal[1]--;
            vertex_normal[2]--;

            vertex_texture[0]--;
            vertex_texture[1]--;
            vertex_texture[2]--;

            tri.point[0] = vertex[0];
            tri.point[1] = vertex[1];
            tri.point[2] = vertex[2];

			tri.vnormal[0] = vertex_normal[0];
			tri.vnormal[1] = vertex_normal[1];
			tri.vnormal[2] = vertex_normal[2];

            triangles.push_back(tri);
        }

        line = "";
    }

    cout<< "mesh meta data: "<<endl;
    cout<< "vertices: "<< vertices.size()<<endl;
    cout<< "faces: "<< triangles.size() <<endl;

    return true;
}

kuro::object::bbox kuro::object::mesh::calculateBB()
{
    bounding_box.low = vec4f(INFINITY, INFINITY, INFINITY);
    bounding_box.high = vec4f(-INFINITY, -INFINITY, -INFINITY);

	/*works out the bounding box of the mesh by finding the largest and smallest
	boundaries for the X, Y and Z axes.*/
    for(int i = 0;i<vertices.size();i++)
    {
        if(vertices[i].x < bounding_box.low.x) bounding_box.low.x = vertices[i].x;
        if(vertices[i].y < bounding_box.low.y) bounding_box.low.y = vertices[i].y;
        if(vertices[i].z < bounding_box.low.z) bounding_box.low.z = vertices[i].z;

        if(vertices[i].x > bounding_box.high.x) bounding_box.high.x = vertices[i].x;
        if(vertices[i].y > bounding_box.high.y) bounding_box.high.y = vertices[i].y;
        if(vertices[i].z > bounding_box.high.z) bounding_box.high.z = vertices[i].z;
    }

    return bounding_box;
}

#endif
