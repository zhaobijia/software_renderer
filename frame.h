#ifndef __FRAME_H__
#define __FRAME_H__
#include "utils.h"

class Frame
{
private:
	int width, height;
	unsigned int* loaded_image;
	unsigned int* framebuffer;
	float* zbuffer;
	int bpp; //8-bit component per pixel for stb lib to load image
public:
	Frame();
	~Frame();
	int get_width();
	int get_height();
	unsigned int* get_framebuffer();
	unsigned int* get_loaded_image();
	void set_framebuffer(unsigned int* buffer);
	void set_pixel(int x, int y, Color color);
	bool load_image(char const* filename);
	friend std::ostream& operator<<(std::ostream& out, const Frame& f);
};

#endif // FRAME_H

