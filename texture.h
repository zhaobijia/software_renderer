#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class Texture
{
private:
	int width, height;
	int bpp; //8-bit component per pixel for stb lib to load image
	unsigned int* loaded_texture;
public:
	Texture(const char* filename);
	Texture(const char* filename, int& w, int& h);
	~Texture();
	unsigned int* get_texture();
	int get_width();
	int get_height();
};

#endif //__TEXTURE_H__