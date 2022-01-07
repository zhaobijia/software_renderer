#include "frame.h"
#include <iostream>
#include <cstring>
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

Frame::Frame():width(0),height(0)
{
	framebuffer = new unsigned int[width * height]{};
	zbuffer = new float[width * height]{};
}
Frame::~Frame()
{
	delete[] loaded_image;
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
