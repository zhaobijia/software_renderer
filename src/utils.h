#ifndef __UTILS_H__
#define __UTILS_H__
#include<algorithm>
#include<iostream>
#include "maths.h"

#define RED Color(255,0,0)
#define GREEN Color(0,255,0)
#define BLUE Color(0,0,255)
#define WHITE Color(255,255,255)

class Color
{
private:
	int r, g, b;
	unsigned int color;
public:
	Color();
	Color(int _r, int _g, int _b);
	~Color();
	int clamp(int _c);
	unsigned int get_color_uint32();
	Color operator +(Color c) { return Color(clamp(r + c.r), clamp(g + c.g), clamp(b + c.b)); }
	
	Color operator *(float t) { return Color(r * t, g * t, b * t); }
	Color operator *(Color c) { return Color(clamp(r * c.r/255),clamp( g * c.g/255), clamp(b * c.b/255)); }
	friend std::ostream& operator<<(std::ostream& out, const Color& c);
};

//for phong lighting model calculate the reflection, reference:
//https://en.wikipedia.org/wiki/Phong_reflection_model
inline float3 reflect(float3& light_in,  float3& normal)
{
	return normal * (light_in.dot(normal) * 2) - light_in;
}

#endif //__UTILS_H__