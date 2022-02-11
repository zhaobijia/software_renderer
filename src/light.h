#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "maths.h"
#include "utils.h"
//reference: https://learnopengl.com/Lighting/Light-casters

struct ILight
{
	float3 position;
	float3 direction;
	Color color;
	ILight() {};
	ILight(float3 pos, float3 dir, Color c) :position(pos),direction(dir),color(c) {};
	ILight(ILight &light) { position = light.position; direction = light.direction; color = light.color; }
	virtual ~ILight() {};

};

//light source is modeled to be infinitely far away, all light rays have the same direction
struct directional_light:ILight
{
	directional_light(float3 pos, float3 dir, Color c):ILight(pos, dir, c){}
};

//A light source with a given position somewhere in a world that illuminates in all directions, where the light rays fade out over distance
struct point_light:ILight
{
	float constant;
	float linear;
	float quadratic;

	point_light(float3 pos, float3 dir, Color c) :ILight(pos, dir, c) {}


};

//A light source that is located somewhere in the environment that only shoots them in a specific direction.
struct spotlight :ILight
{

	float phi;
	spotlight(float3 pos, float3 dir, float p, Color c):ILight(pos,dir,c),phi(p){}
};

#endif