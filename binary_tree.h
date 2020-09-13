#ifndef KURO_BINARY_TREE_H
#define KURO_BINARY_TREE_H

#include <vector>
#include <fstream>
#include "data-structures.h"
#include "util.h"
#include "vec4f.h"
#include "object.h"
#include "bbox.h"
#include "mesh.h"

/*binary used to partition space*/
class kuro::object::binary_tree
{
    public:
        /*bounding box for binary tree node*/
        bbox bounding_box;

        /*if the node has child nodes*/
        bool leaf;

        /*the scope of the node starting from 0 onwards*/
        kuro::util::uint depth;

        /*pointers to the child nodes of the node*/
        binary_tree_cache* child[2];

        /*a dynamic array of the triangles belonging
        to the node*/
        vector <kuro::util::uint> faces;

        /*partitions the current node*/
        void partition(mesh* m, kuro::util::uint max_depth);
};

/*used to save a binary tree reducing build time*/
class kuro::object::binary_tree_cache: public kuro::object::binary_tree
{
    public:
        /*stores the parent of the node*/
        binary_tree_cache* parent;

    public:
        /*adds a line to the file for a node*/
        void save(ofstream& file, int side);

        /*adds a line to the file for a node*/
        void load(string line, binary_tree_cache* iparent);
};

/*implements a mesh that uses a binary tree for intersection testing*/
class kuro::object::btree_mesh: public kuro::object::mesh
{
    public:
        /*the root node of the binary tree*/
        binary_tree_cache mesh_binary_tree;

        /*deals with the intersection with a node*/
        kuro::util::idata node_intersect(kuro::math::ray4f ir, binary_tree* node);

        /*deals with the intersection with node faces*/
        kuro::util::idata faces_intersect(kuro::math::ray4f ir, binary_tree* node);

    public:
        /*gives the maximum depth allowed for the binary tree*/
        kuro::util::uint max_depth;

        /*the intersection routine for using the binary tree*/
        kuro::util::idata intersect(kuro::math::ray4f ir);

        /*initialises the root node of the binary tree*/
        void init_binary_tree();

        /*partitions the mesh into the binary tree*/
        void partition();

        /*saves a binary tree to a file*/
        void save_binary_tree(std::string filename);

        /*loads a binary tree from a file*/
        bool load_binary_tree(std::string filename);
};


#endif
