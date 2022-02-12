#ifndef __SCENE_H__
#define __SCENE_H__
#include "mesh.h"
#include "camera.h"
#include "light.h"
#include "shader.h"

class Scene
{
private:
	directional_light* dir_light = NULL;
	point_light* p_light = NULL;
	spotlight* spot_light = NULL;
public:
	Mesh mesh;
	Texture* texture_ptr = NULL;
	Texture* normal_map_ptr = NULL;
	Camera cam;
	float4x4 mvp ,viewport_matrix;
	ILight light;


	Scene();
	~Scene();
	void init(IShader& shader, int width, int height);
	void update(IShader& shader);
	void load_mesh(const char* filename);
	void load_texture(const char* filename);
	void load_normal_map(const char* filename);

	void set_directional_light(float3 pos, float3 dir, Color color);
	void set_point_light(float3 pos,float3 dir, Color color);
	void set_spotlight(float3 pos, float3 dir, float phi, Color color);
	void set_camera(float3 position, float3 lookat, float3 up, float3 target);
	void set_viewport(int width, int height);
	void update_phong_shader(PhongShader& shader);
	void update_blinn_phong_shader(BlinnPhongShader& shader);
	void calculate_mvp();
};


#endif