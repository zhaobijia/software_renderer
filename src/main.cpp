#include <iostream>
#include "window.h"
#include "mesh.h"
#include "camera.h"

int main()
{	
	//load mesh
	Mesh mesh;
	mesh.read_obj_from_file("assets/african_head.obj");
	Texture texture("assets/african_head_diffuse.tga");
	mesh.set_diffuse_texture(&texture);
	
	//set camera
	Camera cam(float3(0, 0, 0), float3(0, 0, -1), float3(0, 1, 0));
	cam.set_target(float3(0, 0, -2));
	//set mvp 
	float4x4 mvp;
	mvp = mvp.set_mvp(float3(0,0,-2), cam.position, cam.lookat, cam.up, -1,1,-1,1,-100,-1);
	
	TCHAR* title = _T("software_renderer");
		
	Window window(800,800);
	window.set_camera(&cam);

	if (window.initialize_window(title))
		return -1;

	window.frame->flat_shading(&mesh, mvp);
	
	while (window.get_window_exit() == 0) {
		window.window_messages();
		window.frame->clear_buffers();
		mvp = mvp.set_mvp(float3(0, 0, -2), cam.position, cam.lookat, cam.up, -1, 1, -1, 1, -100, -1);
		window.frame->flat_shading(&mesh, mvp);
		window.window_update();
		Sleep(1);
	}
	
	return 0;
}
