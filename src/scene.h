#ifndef __SCENE_H__
#define __SCENE_H__
#include "mesh.h"
#include "camera.h"
#include "light.h"

class Scene
{
private:
	directional_light* dir_light = NULL;
	point_light* p_light = NULL;
	spotlight* spot_light = NULL;
public:
	Mesh mesh;
	Texture* texture_ptr = NULL;
	Camera cam;
	float4x4 mvp;
	ILight light;


	Scene();
	~Scene();
	void init();
	void update();
	void load_mesh(const char* filename);
	void load_texture(const char* filename);
	void set_directional_light(float3 pos, float3 dir, Color color);
	void set_point_light(float3 pos,float3 dir, Color color);
	void set_spotlight(float3 pos, float3 dir, float phi, Color color);
	void set_camera(float3 position, float3 lookat, float3 up, float3 target);
	void calculate_mvp();
};


#endif