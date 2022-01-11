#ifndef __FRAME_H__
#define __FRAME_H__
#include "utils.h"
#include "rasterizer.h"

class Frame
{
private:
	int width, height;
	unsigned int* loaded_image;
	unsigned int* framebuffer;
	float* zbuffer;
	int bpp; //8-bit component per pixel for stb lib to load image
	Rasterizer* rasterizer;
public:
	Frame();
	Frame(int w, int h);
	~Frame();
	int get_width();
	int get_height();
	unsigned int* get_framebuffer();
	unsigned int* get_loaded_image();
	void set_framebuffer(unsigned int* buffer);
	void set_pixel(int x, int y, Color color);
	void set_line(int x0, int y0, int x1, int y1, Color color);
	void set_triangle();
	void wireframe(Mesh* mesh);
	bool load_image(char const* filename);
	friend std::ostream& operator<<(std::ostream& out, const Frame& f);
};

#endif // FRAME_H

