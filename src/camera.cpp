#ifndef KURO_CAMERA_CPP
#define KURO_CAMERA_CPP

#include <math.h>
#include "data-structures.h"
#include "vec2f.h"
#include "vec4f.h"
#include "mat4f.h"
#include "ray4f.h"
#include "camera.h"

using namespace kuro::util;
using namespace kuro::math;

ray4f kuro::camera::camera::get_ray(vec2f screen_pos)
{
	//get values for components of direction
	vec4f d = vec4f();
	d.x = tanf(PI*(fov_x/180.0) * 0.5) * screen_pos.x;
	d.y = tanf(PI*(fov_y/180.0) * 0.5) * screen_pos.y;
	d.z = 1.0;
	d.w = 0.0;

    d = (axis[0]*d.x) + (axis[1]*d.y) + (axis[2]*d.z);

	//normalise the direction
	d = normalise(d);

	return ray4f(p, d);
}


void kuro::camera::camera::translate(vec4f delta)
{
    p = p + delta;
    p.w = 0.0;
}

void kuro::camera::camera::rotate(vec4f iaxis, kuro::util::real theta)
{
    mat4f M = kuro::math::transform::rotate(iaxis, theta);

    axis[0] = normalise(M * axis[0]);
    axis[1] = normalise(M * axis[1]);
    axis[2] = normalise(M * axis[2]);

    p = M * p;
}



ray4f kuro::camera::lens_camera::get_ray(vec2f screen_pos)
{
	screen_pos = -1.0 * screen_pos;

	/*produces a random point on a lens*/
	real theta = util::random::real_random()*2.0*math::PI;
	vec4f sensor_pos = vec4f(0.5*screen_pos.x*s_width, 0.5*screen_pos.y*s_height, -lens_distance);
	vec4f lens_pos = radius*util::random::real_random() * vec4f(cos(theta), sin(theta));
	vec4f focal_length_dir = normalise(-1.0 * sensor_pos);
	vec4f focal_length_pos = focal_length * focal_length_dir;

	vec4f d = focal_length_pos - lens_pos;
	/*transforms the position of the camera to world coordinates
	from relative coordinates*/
	lens_pos = p + (axis[0]*lens_pos.x) + 
				   (axis[1]*lens_pos.y) + 
				   (axis[2]*lens_pos.z);
	
	/*transforms the direction of the ray to world coordinates
	from relative coordinates*/
    d = (axis[0]*d.x) + (axis[1]*d.y) + (axis[2]*d.z);

	//normalise the direction
	d = normalise(d);

	return ray4f(lens_pos, d);
}

#endif
