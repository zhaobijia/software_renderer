#include "maths.h"
#include "utils.h"
#include "mesh.h"

#define WHITE Color(255,255,255)
#define RED Color(255,0,0)
#define GREEN Color(0,255,0)
#define BLUE Color(0,0,255)
typedef struct
{
	int2 min;
	int2 max;
	int area;
}box_t;
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
	void line_sweeping_triangle(int2 p0, int2 p1, int2 p2, Color color);
	box_t bounding_box(int2 p0, int2 p1, int2 p2);
	bool barycentric(int2 pt, int2 t0, int2 t1, int2 t2);
	void barycentric_triangle(int2 p0, int2 p1, int2 p2, box_t& bbox, Color color);
	void draw_triangle(int2 p0, int2 p1, int2 p2, Color color);
	void draw_wireframe(Mesh* mesh);
	void draw_flat_shading(Mesh* mesh);
	void simple_clipping(int h, int w, int* x, int* y);
};


