#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <Windows.h>
#include <tchar.h>
#include "frame.h"

class Window
{
private:
	int width, height;
	static int exit;
	static HWND hwnd;		
	static HDC hdc;			
	static HBITMAP bi_handle;		
	static HBITMAP old_bi_handle;//for deleting
	unsigned int* buffer;		// for framebuffer

public:
	Frame* frame;

	Window( int w, int h);
	Window(Frame* f);
	~Window();


	int get_window_exit();
	int register_wndclass();
	int create_window(int w, int h, const TCHAR* title);
	int initialize_window(int w, int h, const TCHAR* title);	
	int create_frame(int w, int h, HWND hwnd);
	void window_messages(void);
	void window_update();
	void load_image(char const* filename);
	
	
	static LRESULT CALLBACK window_procedure(HWND, UINT, WPARAM, LPARAM);
};

#endif //__WINDOW_H__