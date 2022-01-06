#include "frame.h"
#include <iostream>
#include <cstring>
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

Frame::Frame(int w, int h):width(w),height(h)
{
	framebuffer = new unsigned int[w * h] {};
	zbuffer = new float[w * h]{};
}
Frame::~Frame()
{

	delete[] framebuffer;
	delete[] zbuffer;
}
void Frame::set_pixel(int x, int y, Color color)
{
	framebuffer[x * width + y] = color.get_color_uint32();
}

unsigned int* Frame::get_framebuffer()
{
	return framebuffer;
}

void Frame::set_framebuffer(unsigned int* buffer)
{
	framebuffer = buffer;
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
