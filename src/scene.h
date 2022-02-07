#ifndef __SCENE_H__
#define __SCENE_H__
#include "mesh.h"
#include "camera.h"

class Scene
{
public:
	Mesh mesh;
	Texture* texture_ptr = NULL;
	Camera cam;
	float4x4 mvp;

	Scene();
	~Scene();
	void init();
	void update();
	void load_mesh(const char* filename);
	void load_texture(const char* filename);
	void set_camera(float3 position, float3 lookat, float3 up, float3 target);
	void calculate_mvp();
};


#endif