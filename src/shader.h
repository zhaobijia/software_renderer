#ifndef __SHADER_H__
#define __SHADER_H__

#include "maths.h"
#include "utils.h"

//shader
struct IShader
{
	virtual ~IShader() {};
	//The main goal of the vertex shader is to transform the coordinates of the vertices. The secondary goal is to prepare data for the fragment shader
	virtual float3 vertex(float3 vertex) = 0;
	//The main goal of the fragment shader is to determine the color of the current pixel.
	virtual Color fragment(float3 barycentric, Color color) = 0;
};
//flat shader

struct FlatShader : IShader
{
	float4x4 mvp;

	virtual float3 vertex(float3 vertex)
	{
		return mvp * vertex;
	}

	virtual Color fragment(float3 barycentric, Color color)
	{
		return color;
	}
};
//gouraud shader




#endif