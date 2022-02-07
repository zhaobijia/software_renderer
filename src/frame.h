#ifndef __FRAME_H__
#define __FRAME_H__
#include "utils.h"
#include "rasterizer.h"
#include "shader.h"
#include "scene.h"

class Frame
{
private:
	int width, height;
	unsigned int* framebuffer = NULL;
	float* zbuffer = NULL;
	Rasterizer* rasterizer = NULL;
public:
	Frame();
	Frame(int w, int h);
	~Frame();
	int get_width();
	int get_height();
	unsigned int* get_framebuffer();
	void init_framebuffer(unsigned int* buffer);
	void set_pixel(int x, int y, Color color);
	void set_line(int x0, int y0, int x1, int y1, Color color);
	void set_triangle(float3* tri, Color color);
	void rasterize(Scene& scene, IShader &shader);
	void wireframe(Mesh& mesh, float4x4 mvp);
	void clear_buffers();
	friend std::ostream& operator<<(std::ostream& out, const Frame& f);
};

#endif // FRAME_H

