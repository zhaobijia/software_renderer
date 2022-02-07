#include "frame.h"
#include <iostream>
#include <cstring>


Frame::Frame():width(100),height(100)
{
	zbuffer = new float[width * height]{};
}

Frame::Frame(int w, int h) : width(w), height(h)
{
	zbuffer = new float[width * height]{};
}
Frame::~Frame()
{
	delete[] zbuffer;
}
void Frame::set_pixel(int x, int y, Color color)
{
	assert(rasterizer);
	rasterizer->draw_pixel(x, y, color);
}
void Frame::set_line(int x0, int y0, int x1, int y1, Color color)
{
	assert(rasterizer);
	rasterizer->draw_line(x0, y0, x1, y1, color);
}
void Frame::set_triangle(float3* tri, Color color)
{
	assert(rasterizer);

	//rasterizer->draw_triangle(tri, color);

}
void Frame::wireframe(Mesh& mesh, float4x4 mvp)
{
	assert(rasterizer);
	rasterizer->draw_wireframe(mesh, mvp);
}

void Frame::rasterize(Scene& scene, IShader& shader)
{
	assert(rasterizer);
	rasterizer->rasterize(scene, shader);
}


unsigned int* Frame::get_framebuffer()
{
	return framebuffer;
}

void Frame::init_framebuffer(unsigned int* buffer)
{
	framebuffer = buffer;//point to buffer passed in
	rasterizer = new Rasterizer(width, height, framebuffer, zbuffer);
}

int Frame::get_width()
{
	return width;
}
int Frame::get_height()
{
	return height;
}

void Frame::clear_buffers()
{
	for (int i = 0; i < width * height; i++)
	{
		framebuffer[i] = 0;
		zbuffer[i] = std::numeric_limits<float>::min();
	}
}


