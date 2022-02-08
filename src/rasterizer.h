#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__
#include "maths.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "scene.h"

#define WHITE Color(255,255,255)
#define RED Color(255,0,0)
#define GREEN Color(0,255,0)
#define BLUE Color(0,0,255)
typedef struct
{
	float2 min;
	float2 max;
	float area;
}box_t;
class Rasterizer
{
private:
	int width, height;
	unsigned int* framebuffer = NULL;
	float* zbuffer = NULL;

public:
	Rasterizer(int w, int h, unsigned int* fb, float* zb);
	~Rasterizer();

	void update_framebuffer(unsigned int* buffer);
	void set_zbuffer_value(int x, int y, float t);
	float get_zbuffer_value(int x, int y);
	void set_framebuffer_to_zbuffer();
	void draw_pixel(int x, int y, Color color);
	void draw_pixel(int x, int y, int color);
	void draw_line(int x0, int y0, int x1, int y1, Color color);
	void line_sweeping_triangle(float3 p0, float3 p1, float3 p2, Color color);
	box_t bounding_box(float2 p0, float2 p1, float2 p2);
	float3 barycentric(float3 pt, float3 t0, float3 t1, float3 t2);

	void draw_triangle(float3* tri, IShader& shader);
	void draw_textured_triangle(float3* tri, int2* uv, Texture* texture, IShader& shader);
	void draw_wireframe(Mesh& mesh, float4x4 mvp);
	//void draw_flat_shading(Mesh* mesh, float4x4 mvp);
	void rasterize(Scene& scene, IShader& shader);
	void simple_clipping(int h, int w, int* x, int* y);
};


#endif //__RASTERIZER_H__