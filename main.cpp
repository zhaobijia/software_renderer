#include <iostream>
#include "window.h"
#include "mesh.h"


int main()
{	
	TCHAR* title = _T("Test for win32 to work");
		
	Window window(800,600);

	if (window.initialize_window(800, 600, title))
		return -1;

	window.set_pixel(250, 250, Color(255, 0, 0));
	//so far only absolute path for filename can be loaded.
	window.load_image("assets/test.png");

	while (window.get_window_exit() == 0) {
		window.window_messages();
		window.window_update();
		Sleep(1);
	}
	
	return 0;
}
