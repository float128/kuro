#ifndef KURO_MAT4F_H
#define KURO_MAT4F_H

#include "data-structures.h"
#include "vec4f.h"

using namespace kuro::util;
using namespace kuro::math;

/*stores a 4x4 matrix (a 4x4 allows for translations)*/
struct kuro::math::mat4f
{
	/*
	mat[x][y]
	x is column number starting from the left
	y is row number starting from the top
	*/
	real mat[4][4];
};


/*identity matrix*/
const mat4f mat4f_identity = {1, 0, 0, 0,
							  0, 1, 0, 0,
							  0, 0, 1, 0,
							  0, 0, 0, 1};
/*zero matrix*/
const mat4f mat4f_zero = {0, 0, 0, 0,
						  0, 0, 0, 0,
						  0, 0, 0, 0,
						  0, 0, 0, 0};

/*adds two 4d matrices*/
mat4f operator +(mat4f A, mat4f B);

/*subtracts two 4d matrices*/
mat4f operator -(mat4f A, mat4f B);

/*multiplies two 4d matrices*/
mat4f operator *(mat4f A, mat4f B);

/*multiplies a 4d matrix by a 4d vector*/
vec4f operator *(mat4f A, vec4f d);

/*multiplies a 4d matrix by a constant*/
mat4f operator *(real a, mat4f B);

/*prints a 4d matrix*/
void print(mat4f A);

#endif
