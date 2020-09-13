#ifndef KURO_DATA_STRUCTURES
#define KURO_DATA_STRUCTURES

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <limits>
#include <random>

#define INFINITY std::numeric_limits<double>::infinity()

using namespace std;

/*defines all available data types*/
namespace kuro
{
    /*provides basic utility data types*/
    namespace util
    {
        /*stores light path data for scene traversal*/
        struct path;

        /*stores intersection data*/
        struct idata;

        /*allows for precision of decimal values
        to be changed throughout the source code*/
        typedef double real;

        /*reduces typing necessary*/
        typedef unsigned int uint;

        /*stores a colour value*/
        struct rgba;
    }

    /*provides maths data types*/
    namespace math
    {
        /*stores a 2d vector*/
        struct vec2f;

        /*stores a 4f vector*/
        struct vec4f;

        /*stores a 4d line*/
        struct ray4f;

        /*stores a 4d matrix*/
        struct mat4f;

        /*stores pi value*/
        const util::real PI = 3.14159265359;

		namespace m_util
		{
			/*returns the barycentric coordinates for three points*/
			vec4f barycentric(vec4f a, vec4f b, vec4f c, vec4f x);
		}

        namespace transform
        {
            /*translate the given point by delta*/
            vec4f translate(vec4f p, vec4f delta);

            /*translate the given point by delta*/
            mat4f translate(vec4f delta);

            /*gives the matrix that rotates a given point by theta (in degrees) around the specified axis at the origin*/
            mat4f rotate(vec4f axis, kuro::util::real theta);

            /*scales the given point from the origin for each axis for the given scale factor*/
            vec4f scale(vec4f p, vec4f scale_factor);

            /*scales the given point from the origin for each axis for the given scale factor*/
            mat4f scale(vec4f scale_factor);
        }
    }

    namespace util
    {
        namespace random
        {
            static default_random_engine generator;
            static uniform_real_distribution<real> distribution(0.0, 1.0);

            /*produces a random number in the interval [0.0, 0.0]*/
            real real_random();

            /*produces a random 3d vector less than 90 degrees
            to the specified vector n*/
            math::vec4f vec3f_random(math::vec4f n);
        }
    }

    namespace brdf
    {
        /*stores the BRDF (Bidirectional Reflectance Distribution Function
        of an object*/
        class brdf;

        /*only the colour of the object is displayed*/
        class basic;

        /*BRDF used for ambient occlusion*/
        class ambient_occ;

		/*a normal diffuse BRDF*/
		class normal_diffuse;

		/*a simple reflection BRDF*/
		class simple_reflect;

		/*an emission BRDF*/
        class emission;

        /*a diffuse BRDF*/
        class diffuse;

		/*a mix BRDF (mixes two brdf's)*/
		class mix;
    }


    /*provides object data types inherited from the base object type*/
    namespace object
    {
        /*implements a base object class which holds callback function declarations*/
        class base;

        /*implements a sphere object which inherits the base object class*/
        class sphere;

        /*implements a sphere object which inherits the base object class*/
        class disk;

        /*implements a line object which inherits the base object class*/
        class line;

        /*implements a triangle structure*/
        struct triangle;

        /*implements a mesh object which inherits the base object class*/
        class mesh;

        /*implements a bounding box class*/
        class bbox;

        /*implements an octree for space partitioning*/
        class octree;

        /*implements a binary tree for space partitioning*/
        class binary_tree;

        /*implements a class for saving a binary tree*/
        class binary_tree_cache;

        /*implements a mesh that utilises a binary tree*/
        class btree_mesh;

        /*implements a mesh that utilises a binary tree with caching*/
        class btree_mesh_cache;
    }

    /*provides camera data types inherited from the base camera type*/
    namespace camera
    {
        /*implements a base camera class which holds the callback function declarations*/
        class base;

        /*implements a basic camera which inherits the base camera class*/
        class camera;

		/*implements a camera with focal length and depth of field, inherits the camera::camera class*/
        class lens_camera;
    }

    /*provides image data types inherited from the base image data type*/
    namespace image
    {
        /*implements a base image type which holds the callback function declarations for
        an image*/
        class base;

        /*implements a basic image class which inherits the base image class*/
        class image;

        /*implements the BMP image format which and inherits the image class*/
        class bmp;

    }

    /*provides the scene data types which holds all the information
    that describes a scene*/
    namespace scene
    {
        /*implements a scene structure which holds all the data necessary
        to render a single path traced image*/
        class scene;

        /*implements the settings used to create the image and the settings
        used during rendering*/
        struct render_settings;
    }
}

#endif
