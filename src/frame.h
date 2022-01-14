#ifndef __FRAME_H__
#define __FRAME_H__
#include "utils.h"
#include "rasterizer.h"

class Frame
{
private:
	int width, height;
	unsigned int* framebuffer;
	float* zbuffer;
	Rasterizer* rasterizer;
public:
	Frame();
	Frame(int w, int h);
	~Frame();
	int get_width();
	int get_height();
	unsigned int* get_framebuffer();
	void set_framebuffer(unsigned int* buffer);
	void set_pixel(int x, int y, Color color);
	void set_line(int x0, int y0, int x1, int y1, Color color);
	void set_triangle(float3* tri, Color color);
	void wireframe(Mesh* mesh);
	void flat_shading(Mesh* mesh);
	friend std::ostream& operator<<(std::ostream& out, const Frame& f);
};

#endif // FRAME_H

