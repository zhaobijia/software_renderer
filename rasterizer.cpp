#include "rasterizer.h"

Rasterizer::Rasterizer(int w, int h, unsigned int* fb):width(w),height(h),framebuffer(fb){}

Rasterizer::~Rasterizer(){}

void Rasterizer::update_framebuffer(unsigned int* buffer)
{
    framebuffer = buffer;
}

void Rasterizer::draw_pixel(int x, int y, Color color) {

 	framebuffer[y * width + x] = color.get_color_uint32();
}

void Rasterizer::draw_line(int x0, int y0, int x1, int y1, Color color)
{
    bool steep = false;
    //transpose if line is steep
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    //simple clip
    simple_clipping(height-1, width-1, &x0, &y0);
    simple_clipping(height-1, width-1, &x1, &y1);

    float dx = x1 - x0;
    int dy = y1 - y0;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        
        float t = dx>0?((x - x0) / dx):0;
        y = y0 + dy * t;

        if (steep) {
            draw_pixel(y, x, color);
        }
        else {
            draw_pixel(x, y, color);
        }
    }
}

void Rasterizer::simple_clipping(int h, int w, int* x, int* y)
{
    *x = std::max(0, std::min(*x, h));
    *y = std::max(0, std::min(*y, h));
}
void Rasterizer::draw_triangle()
{

}

void Rasterizer::draw_wireframe(Mesh* mesh)
{
    //for each face(triangle vertex group) in mesh
    for (int i = 0; i < mesh->face_count(); i++)
    {
        int3 f = mesh->get_face(i);
        float3 verts[3];
        verts[0] = mesh->get_vertex(f.x);
        verts[1] = mesh->get_vertex(f.y);
        verts[2] = mesh->get_vertex(f.z);
        
        for (int j = 0; j < 3; j++)
        {
            float3 p0 = verts[j];
            float3 p1 = verts[(j + 1) % 3];

            int x0 = (p0.x + 1.) * width / 2.;
            int y0 = (p0.y + 1.) * height / 2.;
            int x1 = (p1.x + 1.) * width / 2.;
            int y1 = (p1.y + 1.) * height / 2.;

            draw_line(x0, y0, x1, y1, Color(255, 255, 255));
        }


    }
}