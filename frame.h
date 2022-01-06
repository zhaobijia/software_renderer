#ifndef __FRAME_H__
#define __FRAME_H__
#include "utils.h"

class Frame
{
private:
	int width, height;
	unsigned int* framebuffer;
	float* zbuffer;

public:
	Frame(int w, int h);
	~Frame();
	int get_width();
	int get_height();
	unsigned int* get_framebuffer();
	void set_framebuffer(unsigned int* buffer);
	void set_pixel(int x, int y, Color color);
	void load_image();
	friend std::ostream& operator<<(std::ostream& out, const Frame& f);
};

#endif // FRAME_H

