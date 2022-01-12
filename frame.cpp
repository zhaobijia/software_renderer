#include "frame.h"
#include <iostream>
#include <cstring>
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

Frame::Frame():width(0),height(0),bpp(0)
{
	framebuffer = new unsigned int[width * height]{};
	zbuffer = new float[width * height]{};
	rasterizer = new Rasterizer(width, height, framebuffer);
}

Frame::Frame(int w, int h) : width(w), height(h), bpp(0)
{
	framebuffer = new unsigned int[width * height]{};
	zbuffer = new float[width * height]{};
	rasterizer = new Rasterizer(width, height, framebuffer);
}
Frame::~Frame()
{
	delete[] loaded_image;
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
void Frame::set_triangle(int2 p0, int2 p1, int2 p2, Color color)
{

	rasterizer->draw_triangle(p0,p1,p2,color);

}
void Frame::wireframe(Mesh* mesh)
{
	rasterizer->draw_wireframe(mesh);
}

void Frame::flatshading(Mesh* mesh)
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

bool Frame::load_image(char const* filename)
{
	loaded_image = (unsigned int*)stbi_load(filename, &width, &height, &bpp, 0);
	if (!loaded_image)
	{
		perror(filename);
		return false;
	}
	else
	{
		return true;
	}
}
unsigned int* Frame::get_loaded_image()
{

	/*for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			std::cout << "i " << i << " ,j " << j << " " << std::endl;
			std::cout <<std::hex<<" "<< loaded_image[(i * width + j)];
		}
		std::cout << std::endl;
	}*/
	return loaded_image;
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
