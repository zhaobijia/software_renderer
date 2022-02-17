#include <iostream>
#include "window.h"
#include "scene.h"
#include "shader.h"
#include "timer.h"

int main()
{	
	int height = 800;
	int width = 800;
	Scene sample_scene;
	TexturedShader shader;
	sample_scene.init(shader, width, height);

	TCHAR* title = _T("software_renderer");
		
	Window window(width,height);
	window.set_camera(&(sample_scene.cam));

	if (window.initialize_window(title))
		return -1;

	window.frame->rasterize(sample_scene, shader);
	
	while (window.get_window_exit() == 0) {
		clock_t start = clock();

		window.window_messages();
		window.frame->clear_buffers();
		//update:
		sample_scene.update(shader);

		window.frame->rasterize(sample_scene, shader);
		window.window_update();

		clock_t end = clock();
		double frame_time = clock_to_milliseconds(end - start);


		Sleep(1);
	}
	
	return 0;
}
