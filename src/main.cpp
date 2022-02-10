#include <iostream>
#include "window.h"
#include "scene.h"
#include "shader.h"
#include "timer.h"

int main()
{	

	Scene sample_scene;
	sample_scene.init();

	//Set shader
	PhongShader shader;
	shader._mvp = sample_scene.mvp;
	shader._cam_pos = sample_scene.cam.position;
	TCHAR* title = _T("software_renderer");
		
	Window window(800,800);
	window.set_camera(&(sample_scene.cam));

	if (window.initialize_window(title))
		return -1;

	window.frame->rasterize(sample_scene, shader);
	
	while (window.get_window_exit() == 0) {
		clock_t start = clock();

		window.window_messages();
		window.frame->clear_buffers();
		//update:
		sample_scene.update();
		//update shader:
		shader._mvp = sample_scene.mvp;
		shader._cam_pos = sample_scene.cam.position;

		window.frame->rasterize(sample_scene, shader);
		window.window_update();

		clock_t end = clock();
		double frame_time = clock_to_milliseconds(end - start);


		Sleep(1);
	}
	
	return 0;
}
