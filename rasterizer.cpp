#include "rasterizer.h"
#define EPS 0.0001f
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



void Rasterizer::line_sweeping_triangle(int2 p0, int2 p1, int2 p2, Color color)
{
    //sort 3 pts base on y (height)
    if (p2.y > p0.y) std::swap(p0, p2);
    if (p2.y > p1.y) std::swap(p1, p2);
    if (p1.y > p0.y) std::swap(p0, p1);
    
    //fill upper triangle and lower triangle

    int2 short01_dir = p1 - p0;
    int2 short12_dir = p2 - p1;
    int2 long_dir = p2 - p0;
    float short01_y = p1.y - p0.y;
    float short12_y = p2.y - p1.y;
    float long_y = p2.y - p0.y;
    
    for (int j = p0.y; j > p2.y; j--)
    {
        float t01 = short01_y == 0 ? 0 : (j - p0.y )/ short01_y;
        float t12 = short12_y == 0 ? 0 : (j - p1.y) / short12_y;
        float t02 = long_y == 0 ? 0 : (j - p0.y )/ long_y;
        int2 pt_short01 = p0 + short01_dir * t01;
        int2 pt_short12 = p1 + short12_dir * t12;
        int2 pt_long = p0 + long_dir * t02;
        if (j > p1.y)
        {
            //upper
            draw_line(pt_short01.x, j, pt_long.x, j, color);
        }
        else 
        {
            //lower
            draw_line(pt_short12.x, j, pt_long.x, j, color);
        }

    }
   
}
box_t Rasterizer::bounding_box(int2 p0, int2 p1, int2 p2)
{
    box_t box;
    int2 clamp(width, height);
    box.max.x = std::min(clamp.x,std::max(std::max(p0.x,p1.x),p2.x));
    box.max.y = std::min(clamp.y, std::max(std::max(p0.y, p1.y), p2.y));
    box.min.x = std::max(0, std::min(std::min(p0.x, p1.x), p2.x));
    box.min.y = std::max(0, std::min(std::min(p0.y, p1.y), p2.y));
    box.area = (box.max.x - box.min.x) * (box.max.y - box.min.y);
    return box;
}

bool Rasterizer::barycentric(int2 pt, int2 t0, int2 t1, int2 t2)
{
    int2 v0 = t0 - t1;
    int2 v1 = t0 - t2;
    int2 v2 = pt - t0;

    float3 vx(v0.x, v1.x, v2.x);
    float3 vy(v0.y, v1.y, v2.y);
    float3 bary = vx.cross(vy);
    bary = float3(bary.x / bary.z, bary.y / bary.z, 1.);

    return (bary.x >=0-EPS) && (bary.y >= 0-EPS) && (bary.x + bary.y < 1+EPS);
}

void Rasterizer::barycentric_triangle(int2 p0, int2 p1, int2 p2, box_t &bbox,Color color)
{

    for (int i = bbox.min.x; i < bbox.max.x; i++)
    {
        for (int j = bbox.min.y; j < bbox.max.y; j++)
        {
            
            if (barycentric(int2(i, j), p0, p1, p2))
            {

                draw_pixel(i, j, color);
            }
        }
    }

}

void Rasterizer::draw_triangle(int2 p0, int2 p1, int2 p2, Color color)
{
    box_t bbox = bounding_box(p0, p1, p2);

    if (bbox.area > .5f * width * height)
    {
        line_sweeping_triangle(p0, p1, p2, color);
    }
    else
    {
        barycentric_triangle(p0, p1, p2, bbox, color);
    }
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

void Rasterizer::draw_flat_shading(Mesh* mesh)
{
    for (int i = 0; i < mesh->face_count(); i++)
    {
        int3 f = mesh->get_face(i);
        //vertex of a triangle
        float3 verts[3];
        verts[0] = mesh->get_vertex(f.x);
        verts[1] = mesh->get_vertex(f.y);
        verts[2] = mesh->get_vertex(f.z);

        //screen coords:
        int2 screen[3];
        for (int i = 0; i < 3; i++)
        {
            screen[i] = int2((verts[i].x + 1.) * width / 2., (verts[i].y + 1.) * height / 2.);
        }
        //normal of the triangle
        float3 normal = ((verts[2] - verts[0]).cross(verts[1] - verts[0])).normalize();

        float3 _light(0, 0, -1);
        float intensity = _light.dot(normal);
        if (intensity > 0)
        {
            draw_triangle(screen[0], screen[1], screen[2], Color(255 * intensity, 255 * intensity, 255 * intensity));
        }
        
    }
}