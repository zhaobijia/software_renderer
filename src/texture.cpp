#include "texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"


Texture::Texture(const char* filename, int& w, int& h) :width(0), height(0), bpp(0)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &bpp, 4);
	loaded_texture = new unsigned int[width * height];
	loaded_texture_color = new Color[width * height];
	if (!data)
	{
		perror(filename);

	}
	else
	{		
		w = width;
		h = height;
		for (int i = 0; i < height * width * 4; i += 4)
		{
			int data_int = 0;
			int r = data[i];
			int g = data[i + 1];
			int b = data[i + 2];
			loaded_texture[i / 4] = (r << 16) | (g << 8) | (b);
			loaded_texture_color[i / 4] = Color(r, g, b);

		}

		stbi_image_free(data);
	}
}
Texture::Texture(const char* filename) :width(0), height(0), bpp(0) 
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &bpp, 4);
	loaded_texture = new unsigned int[width * height];
	loaded_texture_color = new Color[width * height];
	if (!data)
	{
		perror(filename);

	}
	else
	{
		for (int i = 0; i < height * width * 4; i+=4)
		{
			int data_int = 0;
			int r = data[i];
			int g = data[i + 1];
			int b = data[i + 2];			
			loaded_texture[i / 4]=(r << 16) | (g << 8) | (b);	
			loaded_texture_color[i / 4] = Color(r, g, b);
			//std::bitset<32> pixel_bi((loaded_texture[i / 4]));			
		}		
	}
	stbi_image_free(data);

}

Texture::~Texture()
{

	delete[] loaded_texture;
	delete[] loaded_texture_color;
}

int Texture::get_width()
{
	return width;
}

int Texture::get_height()
{
	return height;
}

unsigned int* Texture::get_texture()
{
	return loaded_texture;
}

Color* Texture::get_texture_color()
{
	return loaded_texture_color;
}