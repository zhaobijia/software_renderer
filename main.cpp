#include <iostream>
#include "window.h"
#include "mesh.h"


int main()
{	
	Mesh mesh;
	mesh.read_obj_from_file("assets/african_head.obj");

	TCHAR* title = _T("software_renderer");
		
	Window window(800,800);

	if (window.initialize_window(800, 800, title))
		return -1;
	window.frame->wireframe(&mesh);

	while (window.get_window_exit() == 0) {
		window.window_messages();
		window.window_update();
		Sleep(1);
	}
	
	return 0;
}
