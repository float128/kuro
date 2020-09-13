#ifndef KURO_BINARY_TREE_CPP
#define KURO_BINARY_TREE_CPP

#include <vector>
#include <iostream>

#include "data-structures.h"
#include "ray4f.h"
#include "binary_tree.h"

void kuro::object::binary_tree::partition(mesh* m, kuro::util::uint max_depth)
{
    child[0] = NULL;
    child[1] = NULL;

    leaf = false;

    /*the function exits if the node contains less
    than 4 triangles of if the max depth
    has been met*/
    if(faces.size() <= 4)leaf = true;
    if(depth >= max_depth)leaf = true;

    if(leaf)return;

    kuro::math::vec4f delta;
    kuro::math::vec4f mid_point;
    kuro::util::uint axis;

    /*the size of the bounding box is increased to
    reduce numerical error*/
    delta = bounding_box.high - bounding_box.low;

    mid_point = bounding_box.high + bounding_box.low;
    mid_point = mid_point * 0.5;

    /*new child nodes are created for the
    current node and the settings are
    applied*/
    child[0] = new binary_tree_cache;
    child[1] = new binary_tree_cache;

    child[0]->depth = depth+1;
    child[1]->depth = depth+1;

    child[0]->bounding_box = bounding_box;
    child[1]->bounding_box = bounding_box;

    /*the bounding boxes of the 2 new
    child nodes are constructed*/
    if(delta.x >= delta.y)
    {
        if(delta.x >= delta.z)
        {
            child[0]->bounding_box.low.x = mid_point.x;
            child[1]->bounding_box.high.x = mid_point.x;
            axis = 0;
        }
        else
        {
            child[0]->bounding_box.low.z = mid_point.z;
            child[1]->bounding_box.high.z = mid_point.z;
            axis = 2;
        }
    }
    else
    {
        if(delta.y >= delta.z)
        {
            child[0]->bounding_box.low.y = mid_point.y;
            child[1]->bounding_box.high.y = mid_point.y;
            axis = 1;
        }
        else
        {
            child[0]->bounding_box.low.z = mid_point.z;
            child[1]->bounding_box.high.z = mid_point.z;
            axis = 2;
        }
    }

    /*every triangle belonging to the node
    is split between the 2 child nodes*/
    for(int i = 0;i<faces.size();i++)
    {
        vec4f a, b, c;

        /*the vertices of a triangle*/
        a = m->vertices[m->triangles[faces[i]].point[0]];
        b = m->vertices[m->triangles[faces[i]].point[1]];
        c = m->vertices[m->triangles[faces[i]].point[2]];

        /*test if the triangle intersects with the
        first child node*/
        if(child[0]->bounding_box.tri_intersect(a, b, c))
        {
            child[0]->faces.push_back(faces[i]);
            /*tests if the triangle intersects with the
            second child node*/
            if(child[1]->bounding_box.tri_intersect(a, b, c))
            {
                child[1]->faces.push_back(faces[i]);
            }
        }
        else
        {
            child[1]->faces.push_back(faces[i]);
        }
    }

    faces.clear();

    /*deletes the first child node if it has no
    faces*/
    if(child[0]->faces.size() == 0)
    {
        delete child[0];
        child[0] = NULL;
    }
    else
    {
        /*partitions the first child node*/
        child[0]->partition(m, max_depth);
    }

    /*deletes the second child node if it has no
    faces*/
    if(child[1]->faces.size() == 0)
    {
        delete child[1];
        child[1] = NULL;
    }
    else
    {
        /*partitions the second child*/
        child[1]->partition(m, max_depth);
    }
}

void kuro::object::binary_tree_cache::save(ofstream& file, int side)
{
    /*saves the depth of the node, and its side
    in a binary tree*/
    file << depth << " ";
    file << side;

    /*prevents the printing of a space when no
    faces are present in the node*/
    if(faces.size() > 0)file << " ";

    /*prints all the faces belonging to the
    node*/
    for(int i = 0;i<faces.size();i++)
    {
        file<< faces[i];
        if(i < faces.size()-1)file<< " ";
    }

    file << endl;

    /*saves the child nodes of the node*/
    if(child[0] != NULL)child[0]->save(file, 0);
    if(child[1] != NULL)child[1]->save(file, 1);
}

void kuro::object::binary_tree_cache::load(string line, binary_tree_cache* iparent)
{
    child[0] = NULL;
    child[1] = NULL;
    leaf = false;

    /*add faces and set depth*/
    int count = 0;
    int side = 0;
    int value = 0.0;
    string buffer = "";

    for(int i = 0;i<line.size();i++)
    {
        if((line[i] == ' ')||(i == line.size()-1))
        {
            if((i == line.size()-1))buffer += line[i];

            value = atoi(buffer.c_str());

            if(count == 0)
            {
                depth = value;
            }
            else if(count == 1)
            {
                side = value;
            }
            else
            {
                faces.push_back(value);
            }

            buffer = "";
            count++;
        }
        else
        {
            buffer += line[i];
        }
    }
    /*add faces and set depth*/

    /*calculate bounding box*/
    {
        vec4f delta = iparent->bounding_box.high - iparent->bounding_box.low;

        vec4f mid_point = iparent->bounding_box.high + iparent->bounding_box.low;
        mid_point = mid_point * 0.5;

        vec4f* modify_bound = NULL;

        bounding_box = iparent->bounding_box;

        if(side == 0)
        {
            modify_bound = &bounding_box.low;
        }
        else
        {
            modify_bound = &bounding_box.high;
        }

        if(delta.x >= delta.y)
        {
            if(delta.x >= delta.z)
            {
                modify_bound->x = mid_point.x;
            }
            else
            {
                 modify_bound->z = mid_point.z;
            }
        }
        else
        {
            if(delta.y >= delta.z)
            {
                 modify_bound->y = mid_point.y;
            }
            else
            {
                 modify_bound->z = mid_point.z;
            }
        }
    }
    /*calculate bounding box*/

    /*let parent point to child*/
    if(side == 0)
    {
        iparent->child[0] = this;
    }
    else
    {
        iparent->child[1] = this;
    }
    /*let parent point to child*/

    if(faces.size() > 0)leaf = true;
}

kuro::util::idata kuro::object::btree_mesh::intersect(kuro::math::ray4f ir)
{
    kuro::util::idata data;
	data.hit = false;


    vec4f p = ir.p;
    vec4f d = ir.d;

    ir.p.w = 1.0;
    ir.d.w = 1.0;

	/*transforms the position and direction vectors by the transformation
	matrix created from the transofrmations specified
	by the user whilst adding the object*/
    ir.p = inv_transform_matrix * ir.p;
    ir.d = inv_transform_matrix * ir.d;
	ir.d = ir.d - (inv_transform_matrix * vec4f(0.0, 0.0, 0.0, 1.0));

	ir.p.w = 0.0;
    ir.d.w = 0.0;

	/*test for an intersecton with the bounding box of the mesh*/
	if(!mesh_binary_tree.bounding_box.intersect(ir))return data;

    data = node_intersect(ir, &mesh_binary_tree);

    data.hit_object = this;

    if(data.hit)
    {
        data.p.w = 1.0;
        data.n.w = 1.0;
		/*transforms the position of intersection and
		normal at the point of intersecton to world coordinates*/
        data.p = transform_matrix * data.p;

        data.n = transform_matrix * data.n;
		data.n = data.n - (transform_matrix * vec4f(0.0, 0.0, 0.0, 1.0));

		data.p.w = 0.0;
		data.n.w = 0.0;

        data.distance = len(p - data.p)/len(d);
    }

    return data;
}

kuro::util::idata kuro::object::btree_mesh::node_intersect(kuro::math::ray4f ir, binary_tree* node)
{
    kuro::util::idata data;
    data.hit = false;

	/*recursive function exits if the node
	is a leaf and tests for intersection with
	the faces contained by the node*/
    if(node->leaf)
    {
        return faces_intersect(ir, node);
    }

	if(node->child[0] != NULL)
	{
		if(node->child[1] == NULL)
		{
			if(node->child[0]->bounding_box.intersect(ir)) return node_intersect(ir, node->child[0]);
		}
		else
		{
			if(node->bounding_box.in_bb(ir.p))
			{
				if(node->child[0]->bounding_box.in_bb(ir.p))
				{
					data = node_intersect(ir, node->child[0]);

					if(data.hit) return data;

					if(node->child[1]->bounding_box.intersect(ir)) return node_intersect(ir, node->child[1]);
				}
				else
				{
					data = node_intersect(ir, node->child[1]);

					if(data.hit) return data;

					if(node->child[0]->bounding_box.intersect(ir)) return node_intersect(ir, node->child[0]);
				}
			}
			else
			{
				kuro::util::real distance[2];

				if(node->child[0]->bounding_box.intersect(ir, &distance[0]))
				{
					if(node->child[1]->bounding_box.intersect(ir, &distance[1]))
					{
						if(distance[0] < distance[1])
						{
							data = node_intersect(ir, node->child[0]);

							if(data.hit) return data;

							return node_intersect(ir, node->child[1]);
						}
						else
						{
							data = node_intersect(ir, node->child[1]);

							if(data.hit) return data;

							return node_intersect(ir, node->child[0]);
						}
					}
					else
					{
						return node_intersect(ir, node->child[0]);
					}
				}
				else
				{
					if(node->child[1]->bounding_box.intersect(ir)) return node_intersect(ir, node->child[1]);
				}
			}
		}
	}
	else
	{
		if(node->child[1] != NULL)
		{
			if(node->child[1]->bounding_box.intersect(ir)) return node_intersect(ir, node->child[1]);
		}
	}

    return data;
}

kuro::util::idata kuro::object::btree_mesh::faces_intersect(kuro::math::ray4f ir, binary_tree* node)
{
    idata data;
    idata tmp_data;

    data.hit = false;

	/*tests if the input ray intersects with any of triangles
	belonging to the input node*/
    for(kuro::util::uint i = 0;i < node->faces.size();i++)
    {
		/*two different routines are used to test for an intersection
		with a triangle, when smooth shading is enabled barycentric
		coordinates are used to linearly interpolate the normals
		at the vertices of the triangle*/
        if(smooth_shading)
        {
            tmp_data = triangles[node->faces[i]].intersect(ir, &vertices[0], &vnormals[0]);
        }
        else
        {
            tmp_data = triangles[node->faces[i]].intersect(ir, &vertices[0]);
        }

        if(!tmp_data.hit)continue;

        if((len(tmp_data.p - ir.p)<len(data.p - ir.p))||(!data.hit))data = tmp_data;
    }

    return data;
}

void kuro::object::btree_mesh::init_binary_tree()
{
    mesh_binary_tree.bounding_box = calculateBB();

    mesh_binary_tree.depth = 0;

    for(kuro::util::uint i = 0;i<triangles.size();i++)
    {
        mesh_binary_tree.faces.push_back(i);
    }
}

void kuro::object::btree_mesh::partition()
{
    mesh_binary_tree.partition(this, max_depth);
}

void kuro::object::btree_mesh::save_binary_tree(std::string filename)
{
    ofstream ofile(filename);

    mesh_binary_tree.save(ofile, -1);

    ofile.close();
}

bool kuro::object::btree_mesh::load_binary_tree(std::string filename)
{
    mesh_binary_tree.bounding_box = calculateBB();
    mesh_binary_tree.child[0] = NULL;
    mesh_binary_tree.child[1] = NULL;
    mesh_binary_tree.depth = 0;
    mesh_binary_tree.leaf = false;

    ifstream ifile(filename);
    string line = "";

    kuro::object::binary_tree_cache* node = NULL;
    int depth = 0;

    vector <kuro::object::binary_tree_cache*> parents;
    parents.push_back(&mesh_binary_tree);

    while(ifile)
    {
        getline(ifile, line);
        if(line == "")break;

        depth = atoi(line.c_str());

        if(depth == 0)continue;

        if(depth <= parents.back()->depth)
        {
            while(true)
            {
                if(parents.back()->depth >= depth) parents.pop_back();

                if(parents.back()->depth == depth - 1)break;
            }
        }

        node = new kuro::object::binary_tree_cache;

        node->load(line, parents.back());

        if(depth > parents.back()->depth)
        {
            parents.push_back(node);
        }
    }

    ifile.close();

    if(depth != 0)return true;

    return false;
}

#endif
