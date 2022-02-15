#include "scene.h"

Scene::Scene(){}
Scene::~Scene()
{
	delete texture_ptr;
	delete dir_light;
	delete p_light;
	delete spot_light;
}
void Scene::init(IShader& shader, int width, int height)
{
	//Hard coded sample scene here
	//--> todo: change to reading scene config file
	load_mesh("african_head.obj");
	load_texture("african_head_diffuse.tga");
	load_normal_map("african_head_nm_tangent.png");

	set_directional_light(float3(0,0,0),float3(-1, -1, -1), WHITE);
	set_camera(float3(0, 0, 0), float3(0, 0, -1), float3(0, 1, 0), float3(0, 0, -2));
	set_viewport(width,height);
	calculate_matrices();
	//set up type of shader

	update_textured_shader((TexturedShader&)shader);
	//update_blinn_phong_shader((BlinnPhongShader&)shader);
}
void Scene::update(IShader& shader)
{
	//default orbit cam
	cam.auto_orbit_horizontal(0.01f);
	calculate_matrices();
	update_textured_shader((TexturedShader&)shader);
	//update_blinn_phong_shader((BlinnPhongShader&)shader);
}

void Scene::load_mesh(const char* filename)
{
	mesh.read_obj_from_file(filename);
}
void Scene::load_texture(const char* filename)
{
	texture_ptr = new Texture(filename);
	mesh.set_diffuse_texture(texture_ptr);

}

void Scene::load_normal_map(const char* filename)
{
	normal_map_ptr = new Texture(filename);
	mesh.set_normal_texture(normal_map_ptr);
}

void Scene::set_directional_light(float3 pos, float3 dir, Color color)
{
	
	dir_light = new directional_light(pos, dir, color);
	light = *dir_light;

}
void Scene::set_point_light(float3 pos, float3 dir, Color color)
{

	p_light = new point_light(pos,dir, color);
	light = *p_light;

}
void Scene::set_spotlight(float3 pos, float3 dir, float phi, Color color)
{
	spot_light = new spotlight(pos, dir, phi, color);
	light = *spot_light;
}
void Scene::set_camera(float3 position, float3 lookat, float3 up, float3 target)
{
	cam.position = position;
	cam.lookat = lookat;
	cam.up = up;
	cam.set_target(target);
}

void Scene::set_viewport(int width, int height)
{

	viewport_matrix = viewport_matrix.set_viewport(width, height);
}
void Scene::calculate_matrices()
{
	m = m.set_model(cam.target);
	mv = (mv.set_model_view(cam.position, cam.lookat, cam.up))*m;
	mvp = mvp.set_mvp(cam.target, cam.position, cam.lookat, cam.up, cam.left, cam.right, cam.bottom, cam.top, cam.far_plane, cam.near_plane);

}

void Scene::update_phong_shader(PhongShader& shader)
{
	shader._mvp =mvp;
	shader._cam_pos = cam.position;

	ILight light_update (light);
	float3 pos = mvp.mul(light.position,1);
	float3 pos_dir = light.position + light.direction;
	light_update.direction = mvp.mul(pos_dir,1) - pos;
	shader._light = light_update;
}

void Scene::update_blinn_phong_shader(BlinnPhongShader& shader)
{

	ILight light_update(light);
	float3 pos = mvp.mul(light.position,1);
	float3 pos_dir = light.position + light.direction;
	light_update.direction = mvp.mul(pos_dir,0) - pos;

	shader._mvp = mvp;
	shader._m = m;
	shader._cam_pos = cam.position;
	shader._light = light_update;
}

void Scene::update_textured_shader(TexturedShader& shader)
{
	ILight light_update(light);
	float3 pos = mvp.mul(light.position,1);
	float3 pos_dir = light.position + light.direction;
	light_update.direction = mvp.mul(pos_dir,0) - pos;

	shader._mvp = mvp;
	shader._mv = mv;
	shader._m = m;
	shader._cam_pos = cam.position;
	shader._light = light_update;
}