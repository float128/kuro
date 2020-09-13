#ifndef KURO_CAMERA_H
#define KURO_CAMERA_H

#include "data-structures.h"
#include "vec4f.h"
#include "ray4f.h"

using namespace kuro::util;
using namespace kuro::math;

/*base class upon which cameras are built*/
class kuro::camera::base
{
    public:
        /*returns a ray that passes through a position
        on the screen of the camera*/
        virtual ray4f get_ray(vec2f screen_pos) = 0;

        //translates the camera by delta
        virtual void translate(vec4f delta){};

        //rotates the camera about the origin and axis
        virtual void rotate(vec4f axis, kuro::util::real theta){};
};

class kuro::camera::camera: public kuro::camera::base
{
    public:
        /*returns a ray that passes through a position
        on the screen of the camera
        from a focal point*/
        ray4f get_ray(vec2f screen_pos);

        //translates the camera by delta
        void translate(vec4f delta);

        //rotates the camera about the origin and axis
        void rotate(vec4f axis, kuro::util::real theta);

        /*stores the horizontal and vertical
        field of view*/
        real fov_x, fov_y;

        /*stores the position of the camera*/
        vec4f p;

        /*stores the axis of the camera*/
        vec4f axis[3];
};

class kuro::camera::lens_camera: public kuro::camera::camera
{
	private:
		
    public:
		/*returns a ray that passes through a position
        on the screen of the camera
        from a focal point*/
        ray4f get_ray(vec2f screen_pos);

		/*stores the radius of the lens*/
		real radius;

		/*stores the focal length of the camera*/
		real focal_length;

		/*stores the distance of the lens from the image sensor*/
		real lens_distance;

		/*stores the sensor dimensions*/
		real s_width, s_height;
};

#endif
