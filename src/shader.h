#ifndef __SHADER_H__
#define __SHADER_H__
#undef max
#include "maths.h"
#include "utils.h"
#include "light.h"
//shader
struct IShader
{
	virtual ~IShader() {};
	//The main goal of the vertex shader is to transform the coordinates of the vertices. The secondary goal is to prepare data for the fragment shader
	virtual float3 vertex(int nthvert, float3 vertex, float3 normal, ILight light) = 0;
	//The main goal of the fragment shader is to determine the color of the current pixel.
	virtual Color fragment(float3 barycentric, Color color) = 0;
};
//flat shader

struct FlatShader : IShader
{
	float4x4 mvp;
	float3 _normal;
	float _intensity;
	
	virtual float3 vertex(int nthvert, float3 vertex, float3 normal, ILight light)
	{
		_normal = (mvp * normal).normalize();
		_intensity = (light.direction.dot(_normal));
		return mvp * vertex;
	}

	virtual Color fragment(float3 barycentric, Color color)
	{

		return color *_intensity;
	}
};
//gouraud shader


struct GouraudShader : IShader
{
	float4x4 _mvp;
	float3 _normal;
	float _varying_intensity[3];
	ILight _light;

	virtual float3 vertex(int v_idx, float3 vertex, float3 normal, ILight light)
	{
		_normal = (_mvp * normal);//.normalize();		
		float intensity =-light.direction.normalize().dot(_normal);	
		_varying_intensity[v_idx] = std::max(0.0f, intensity);
		_light = light;
		return _mvp * vertex;
	}

	virtual Color fragment(float3 bary, Color color)
	{
		float interpolated_intensity = _varying_intensity[0] + (_varying_intensity[1] - _varying_intensity[0]) * bary.x + (_varying_intensity[2] - _varying_intensity[0]) * bary.y;		

		return color*interpolated_intensity;
	}
};


#endif