#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "utils.h"
class Texture
{
private:
	int width, height;
	int bpp; //8-bit component per pixel for stb lib to load image
	unsigned int* loaded_texture;
	Color* loaded_texture_color;
public:
	Texture(const char* filename);
	Texture(const char* filename, int& w, int& h);
	~Texture();
	unsigned int* get_texture();
	Color* get_texture_color();
	int get_width();
	int get_height();
};

#endif //__TEXTURE_H__