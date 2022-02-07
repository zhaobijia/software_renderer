#include "scene.h"

Scene::Scene(){}
Scene::~Scene()
{
	delete texture_ptr;
	
}
void Scene::init()
{
	//Hard coded sample scene here
	//--> todo: change to reading scene config file
	load_mesh("african_head.obj");
	load_texture("assets/african_head_diffuse.tga");
	set_camera(float3(0, 0, 0), float3(0, 0, -1), float3(0, 1, 0), float3(0, 0, -2));
	calculate_mvp();
}
void Scene::update()
{
	//default orbit cam
	cam.auto_orbit_horizontal(0.01f);
	mvp = mvp.set_mvp(cam.target, cam.position, cam.lookat, cam.up, cam.left, cam.right, cam.bottom, cam.top, cam.far_plane, cam.near_plane);
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
void Scene::set_camera(float3 position, float3 lookat, float3 up, float3 target)
{
	cam.position = position;
	cam.lookat = lookat;
	cam.up = up;
	cam.set_target(target);
}
void Scene::calculate_mvp()
{
	mvp = mvp.set_mvp(cam.target, cam.position, cam.lookat, cam.up, cam.left, cam.right, cam.bottom, cam.top, cam.far_plane, cam.near_plane);
}