#ifndef KURO_MAT4F_CPP
#define KURO_MAT4F_CPP

#include <iostream>

#include "data-structures.h"
#include "vec4f.h"
#include "mat4f.h"

using namespace kuro::util;
using namespace kuro::math;

mat4f operator +(mat4f A, mat4f B)
{
	mat4f C;

	for(int j = 0;j<4;j++)
	{
		for(int i = 0;i<4;i++)
		{
			C.mat[i][j] = A.mat[i][j] + B.mat[i][j];
		}
	}

	return C;
}

mat4f operator -(mat4f A, mat4f B)
{
	mat4f C;

	for(int j = 0;j<4;j++)
	{
		for(int i = 0;i<4;i++)
		{
			C.mat[i][j] = A.mat[i][j] - B.mat[i][j];
		}
	}

	return C;
}

mat4f operator *(mat4f A, mat4f B)
{
	mat4f C = mat4f_zero;

	for(int j = 0;j<4;j++)
	{
		for(int i = 0;i<4;i++)
		{
			for(int k = 0;k<4;k++)
			{
				C.mat[i][j] += A.mat[k][j] * B.mat[i][k];
			}
		}
	}

	return C;
}

vec4f operator *(mat4f A, vec4f b)
{
	real d[4] = {b.x, b.y, b.z, b.w};
	real c[4] = {0.0, 0.0, 0.0, 0.0};

	for(int j = 0;j<4;j++)
	{
		for(int i = 0;i<4;i++)
		{
				c[j] += A.mat[i][j] * d[i];
		}
	}

	return vec4f(c[0], c[1], c[2], c[3]);
}

mat4f operator *(real a, mat4f B)
{
	for(int j = 0;j<4;j++)
	{
		for(int i = 0;i<4;i++)
		{
			B.mat[i][j] *= a;
		}
	}

	return B;
}

void print(mat4f A)
{
	std::cout<<"mat4f"<<endl;
	for(int j = 0;j<4;j++)
	{
		std::cout<< "| ";
		for(int i = 0;i<4;i++)
		{
			cout<< A.mat[i][j];
			if(i!=3)cout<<" ";
		}
		std::cout<< " |";
		if(j!=3)cout<<endl;
	}
}

#endif
