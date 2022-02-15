#include "rasterizer.h"
#include <bitset>
#define EPS 0.001f


Rasterizer::Rasterizer(int w, int h, unsigned int* fb, float* zb)
    :width(w),height(h),framebuffer(fb),zbuffer(zb)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            zbuffer[y * width + x] = std::numeric_limits<float>::min();
        }
    }
}

Rasterizer::~Rasterizer() {}



void Rasterizer::update_framebuffer(unsigned int* buffer)
{
    framebuffer = buffer;
}


void Rasterizer::set_zbuffer_value(int x, int y, float t)
{
    zbuffer[y * width + x] = t;
}

float Rasterizer::get_zbuffer_value(int x, int y)
{
    return zbuffer[y * width + x];
}

void Rasterizer::set_framebuffer_to_zbuffer()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float z = zbuffer[width * y + x];
            if (z > 0)
            {    
                z = 255 * (z / height);
                z = Color(z, z, z).get_color_uint32();
            }
            framebuffer[width * y + x] = z;
            
        }
    }
}

void Rasterizer::draw_pixel(int x, int y, Color color)
{
    framebuffer[y * width + x] = color.get_color_uint32();
}

void Rasterizer::draw_pixel(int x, int y, int color)
{
    framebuffer[y * width + x] = color;
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



void Rasterizer::line_sweeping_triangle(float3 p0, float3 p1, float3 p2, Color color)
{
    //sort 3 pts base on y (height)
    if (p2.y > p0.y) std::swap(p0, p2);
    if (p2.y > p1.y) std::swap(p1, p2);
    if (p1.y > p0.y) std::swap(p0, p1);
    
    //fill upper triangle and lower triangle

    float3 short01_dir = p1 - p0;
    float3 short12_dir = p2 - p1;
    float3 long_dir = p2 - p0;
    float short01_y = p1.y - p0.y;
    float short12_y = p2.y - p1.y;
    float long_y = p2.y - p0.y;
    
    for (int j = p0.y; j > p2.y; j--)
    {
        float t01 = short01_y == 0 ? 0 : (j - p0.y )/ short01_y;
        float t12 = short12_y == 0 ? 0 : (j - p1.y) / short12_y;
        float t02 = long_y == 0 ? 0 : (j - p0.y )/ long_y;
        float3 pt_short01 = p0 + short01_dir * t01;
        float3 pt_short12 = p1 + short12_dir * t12;
        float3 pt_long = p0 + long_dir * t02;
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
box_t Rasterizer::bounding_box(float2 p0, float2 p1, float2 p2)
{
    box_t box;
    float2 clamp(width, height);
    box.max.x = std::min(clamp.x,std::max(std::max(p0.x,p1.x),p2.x));
    box.max.y = std::min(clamp.y, std::max(std::max(p0.y, p1.y), p2.y));
    box.min.x = std::max(0.f, std::min(std::min(p0.x, p1.x), p2.x));
    box.min.y = std::max(0.f, std::min(std::min(p0.y, p1.y), p2.y));
    box.area = (box.max.x - box.min.x) * (box.max.y - box.min.y);
    return box;
}

float3 Rasterizer::barycentric(float3 pt, float3 t0, float3 t1, float3 t2)
{
    float3 v0 = t1 - t0; 
    float3 v1 = t2 - t0;
    float3 v2 = t0 - pt;

    float3 vx(v0.x, v1.x, v2.x);
    float3 vy(v0.y, v1.y, v2.y);
    float3 bary = vx.cross(vy);
    if (std::abs(bary.z) < EPS)
    {
        return float3(-1.f, 1.f, 1.f);
    }
    else
    {
        bary = float3(bary.x / bary.z, bary.y / bary.z, 1.);

        return bary;
    }
    
}


void Rasterizer::draw_triangle(float3* tri, IShader& shader, Scene& scene)
{
    float2 pt0(tri[0].x, tri[0].y);
    float2 pt1(tri[1].x, tri[1].y);
    float2 pt2(tri[2].x, tri[2].y);
    box_t bbox = bounding_box(pt0, pt1, pt2);

    for (int i = bbox.min.x; i < bbox.max.x; i++)
    {
        for (int j = bbox.min.y; j < bbox.max.y; j++)
        {
            float3 _pt(i, j, 0);//zvalue can only be found after barycentric
            float3 bary = barycentric(_pt, tri[0], tri[1], tri[2]);//x = u, y = v, z=1
            //check inside triangle
            if (bary.x >= 0 - EPS && bary.y >= 0 - EPS && (bary.x + bary.y) <= 1.f + EPS)
            {
                float3 pt = tri[0] + (tri[1] - tri[0]) * bary.x + (tri[2] - tri[0]) * bary.y;
                pt.z = reverse_depth(pt.z, scene.cam.near_plane, scene.cam.far_plane);
                if (pt.z > get_zbuffer_value(i, j))
                {     
                    Color color = shader.fragment(bary, Color(255, 255, 255));

                    set_zbuffer_value(i, j, pt.z);
                    draw_pixel(i, j, color);
                }

            }
        }
    }
    
}

//void Rasterizer::draw_textured_triangle(float3* tri, int2* uv, Texture* uv_map, IShader& shader, Scene& scene)
//{
//    float2 pt0(tri[0].x, tri[0].y);
//    float2 pt1(tri[1].x, tri[1].y);
//    float2 pt2(tri[2].x, tri[2].y);
//    box_t bbox = bounding_box(pt0, pt1, pt2);
//    Color* texture = uv_map->get_texture_color();
//    int texture_width = uv_map->get_width();
//    int texture_height = uv_map->get_height();
//
//
//    for (int i = bbox.min.x; i < bbox.max.x; i++)
//    {
//        for (int j = bbox.min.y; j < bbox.max.y; j++)
//        {
//            float3 _pt(i, j, 0);//zvalue can only be found after barycentric
//            float3 bary = barycentric(_pt, tri[0], tri[1], tri[2]);//x = u, y = v, z=1
//
//            //check inside triangle
//            if (bary.x >= 0 - EPS && bary.y >= 0 - EPS && (bary.x + bary.y) <= 1.f + EPS)
//            {
//                float3 pt = tri[0] + (tri[1] - tri[0]) * bary.x + (tri[2] - tri[0]) * bary.y;
//                int2 uv_coord = uv[0] + (uv[1] - uv[0]) * bary.x + (uv[2] - uv[0]) * bary.y;
//
//                Color tex_color = texture[uv_coord.y * texture_width + uv_coord.x];
//                pt.z = reverse_depth(pt.z,scene.cam.near_plane, scene.cam.far_plane);
//                if (pt.z > get_zbuffer_value(i, j))
//                {
//                    Color color = shader.fragment(bary, tex_color);
//                    //update z buffer
//
//                    set_zbuffer_value(i, j, pt.z);
//                    draw_pixel(i, j, color);
//                }
//
//            }
//        }
//    }
//
//}


void Rasterizer::draw_wireframe(Mesh& mesh, float4x4 mvp)
{
    //for each face(triangle vertex group) in mesh
    for (int i = 0; i < mesh.face_count(); i++)
    {

        float3 verts[3];


        for (int j = 0; j < 3; j++)
        {
            verts[j] = mesh.get_vertex_with_face_idx(i, j);
            float3 p0 = mvp.mul(verts[j],1);
            float3 p1 = mvp.mul(verts[(j + 1) % 3],1);

            int x0 = (p0.x + 1.) * width / 2.;
            int y0 = (p0.y + 1.) * height / 2.;
            int x1 = (p1.x + 1.) * width / 2.;
            int y1 = (p1.y + 1.) * height / 2.;

            draw_line(x0, y0, x1, y1, Color(255, 255, 255));
        }


    }
}


void Rasterizer::rasterize(Scene& scene, IShader& shader)
{

    if (scene.mesh.has_diffuse_texture())
    {
        //rasterize with textures~
        //load textures here
        Texture* uv_map = scene.mesh.get_diffuse_texture();
        Texture* normal_map = scene.mesh.get_normal_texture();
        TexturedShader& t_shader = (TexturedShader&)shader;

        t_shader.texture_map = uv_map;
        t_shader.normal_map = normal_map;
        for (int i = 0; i < scene.mesh.face_count(); i++)
        {
            //the 3 vertices of a triangle
            float3 verts[3], normals[3];
            float2 uvs[3];
            float3 tangent;
            //screen coordinates(change to float3 b/c adding zbuffer)
            float3 screen_coord[3];
            tangent = scene.mesh.get_tangent(i);
            for (int j = 0; j < 3; j++)
            {
                verts[j] = scene.mesh.get_vertex_with_face_idx(i, j);
                normals[j] = scene.mesh.get_normal_with_face_idx(i, j);//normal from mesh not normal map
                uvs[j] = scene.mesh.get_uv_with_face_idx(i, j);
                //vertex shader applies:

                verts[j] = shader.vertex(j, verts[j], normals[j], uvs[j], tangent);
                screen_coord[j] = scene.viewport_matrix.mul(verts[j],1);
            }
            //back face culling check
            float3 screen_normal = ((screen_coord[2] - screen_coord[0]).cross(screen_coord[1] - screen_coord[0])).normalize();
            
            bool backface = screen_normal.dot(scene.light.direction);
            if (backface)
            {                             
                //passing screen coords will cause perspective incorrect texture mapping ,fix: (TODO)
                draw_triangle(screen_coord, t_shader, scene);
            }
        }
        
    }
    else
    {
        //rasterize without texture mapping :(
        for (int i = 0; i < scene.mesh.face_count(); i++)
        {
            //the 3 vertices of a triangle
            float3 verts[3], normals[3];
            float2 uvs[3];
            float3 tangent;
            //calculate normal if normal is not provided in the model
            //float3 normal = ((verts[2] - verts[0]).cross(verts[1] - verts[0])).normalize();
            //screen coordinates(change to float3 b/c adding zbuffer)
            float3 screen_coord[3];
            for (int j = 0; j < 3; j++)
            {
                verts[j] = scene.mesh.get_vertex_with_face_idx(i, j);
                normals[j] = scene.mesh.get_normal_with_face_idx(i, j);
                uvs[j] = scene.mesh.get_uv_with_face_idx(i, j);
                //vertex shader applies:
                verts[j] = shader.vertex(j, verts[j], normals[j], uvs[j], tangent);
                screen_coord[j] = scene.viewport_matrix.mul(verts[j],1);
            }

            float3 screen_normal = ((screen_coord[2] - screen_coord[0]).cross(screen_coord[1] - screen_coord[0])).normalize();
            bool backface = screen_normal.dot(scene.light.direction);

            if (backface)
            {
                draw_triangle(screen_coord, shader, scene);
            }
        }

    }


}

//reference: https://learnopengl.com/Advanced-OpenGL/Depth-testing
float Rasterizer::reverse_depth(float depth, float near, float far)
{
    float z = depth * 2.f - 1.f;
    return  z+ 2.f * near * far/(far-near);
}

