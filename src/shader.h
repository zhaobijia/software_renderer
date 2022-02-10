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

//toon shader?
struct ToonShader : IShader
{
	float4x4 _mvp;
	float3 _normal;
	float3 _varying_normals[3];
	ILight _light;

	virtual float3 vertex(int v_idx, float3 vertex, float3 normal, ILight light)
	{
		_normal = (_mvp * normal);// .normalize();
		_varying_normals[v_idx] = _normal;
		_light = light;
		return _mvp * vertex;
	}

	virtual Color fragment(float3 bary, Color color)
	{
		//interpolate  normals are slower
		float3 n = _varying_normals[0] + (_varying_normals[1] - _varying_normals[0]) * bary.x + (_varying_normals[2] - _varying_normals[0]) * bary.y;
		float intensity = -_light.direction.dot(n);
		if (intensity > .85) intensity = 1;
		else if (intensity > .60) intensity = .80;
		else if (intensity > .45) intensity = .60;
		else if (intensity > .30) intensity = .45;
		else if (intensity > .15) intensity = .30;
		else intensity = 0;
		Color _color(255, 200, 0);
		return _color * intensity;
	}
};

//phong shading, reference:
//https://learnopengl.com/Lighting/Basic-Lighting

struct PhongShader : IShader
{
	float4x4 _mvp;
	float3 _normal;
	float _varying_intensity[3];
	float3 _varying_pos[3];
	float3 _varying_normals[3];
	ILight _light;
	float3 _cam_pos;

	//phong light model
	float _ambient_intensity;
	Color _ambient;

	float _diffuse_intensity; 
	Color _diffuse;

	float _specular_intensity;
	Color _specular;

	virtual float3 vertex(int v_idx, float3 vertex, float3 normal, ILight light)
	{
		_normal = (_mvp * normal);// .normalize();
		float intensity = -light.direction.dot(_normal);
		_varying_intensity[v_idx] = std::max(0.0f, intensity);
		_varying_pos[v_idx] = vertex;
		_varying_normals[v_idx] = _normal;
		_light = light;
		_ambient = _light.color;
		_diffuse = _light.color;
		_specular = _light.color;
		return _mvp * vertex;
	}

	virtual Color fragment(float3 bary, Color color)
	{

		//lightings:
		_ambient_intensity = 0.1f;
		Color ambient = _ambient * _ambient_intensity;

		_diffuse_intensity = _varying_intensity[0] + (_varying_intensity[1] - _varying_intensity[0]) * bary.x + (_varying_intensity[2] - _varying_intensity[0]) * bary.y;
		Color diffuse =_diffuse* _diffuse_intensity;

		float3 pos = _varying_pos[0] + (_varying_pos[1] - _varying_pos[0]) * bary.x + (_varying_pos[2] - _varying_pos[0]) * bary.y;
		float3 normal = _varying_normals[0] + (_varying_normals[1] - _varying_normals[0]) * bary.x + (_varying_normals[2] - _varying_normals[0]) * bary.y;
		float3 view_dir = (_cam_pos-pos).normalize();
		float3 reflect_dir = reflect(_light.direction, normal).normalize();
		_specular_intensity = std::max(view_dir.dot(reflect_dir), 0.f);
		Color specular = _specular * _specular_intensity;

		return (diffuse + ambient+specular )*color;
	}
};

//

#endif