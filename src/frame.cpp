#include "frame.h"
#include <iostream>
#include <cstring>


Frame::Frame():width(100),height(100)
{
	framebuffer = new unsigned int[width * height]{};
	zbuffer = new float[width * height]{};
	rasterizer = new Rasterizer(width, height, framebuffer,zbuffer);
}

Frame::Frame(int w, int h) : width(w), height(h)
{
	framebuffer = new unsigned int[width * height]{};
	zbuffer = new float[width * height]{};
	rasterizer = new Rasterizer(width, height, framebuffer,zbuffer);
}
Frame::~Frame()
{
	delete[] framebuffer;
	delete[] zbuffer;
}
void Frame::set_pixel(int x, int y, Color color)
{
	rasterizer->draw_pixel(x, y, color);
}
void Frame::set_line(int x0, int y0, int x1, int y1, Color color)
{
	rasterizer->draw_line(x0, y0, x1, y1, color);
}
void Frame::set_triangle(float3* tri, Color color)
{

	rasterizer->draw_triangle(tri, color);

}
void Frame::wireframe(Mesh* mesh)
{
	rasterizer->draw_wireframe(mesh);
}

void Frame::flat_shading(Mesh* mesh)
{
	rasterizer->draw_flat_shading(mesh);
}

unsigned int* Frame::get_framebuffer()
{
	return framebuffer;
}

void Frame::set_framebuffer(unsigned int* buffer)
{
	framebuffer = buffer;//point to buffer
	rasterizer->update_framebuffer(buffer);
}

int Frame::get_width()
{
	return width;
}
int Frame::get_height()
{
	return height;
}


std::ostream& operator<<(std::ostream& out, const Frame& f)
{
	for (int i = 0; i < f.height; i++)
	{
		for (int j = 0; j < f.width; j++)
		{
			out << f.framebuffer[i * f.width + j] << ", ";
		}
	}
	return out;
}
