#include "maths.h"
#include "utils.h"
#include "mesh.h"
class Rasterizer
{
private:
	int width, height;
	unsigned int* framebuffer;
public:
	Rasterizer(int w, int h, unsigned int* fb);
	~Rasterizer();

	void update_framebuffer(unsigned int* buffer);
	void draw_pixel(int x, int y, Color color);
	void draw_line(int x0, int y0, int x1, int y1, Color color);
	void draw_triangle();
	void draw_wireframe(Mesh* mesh);
	void simple_clipping(int h, int w, int* x, int* y);
};

