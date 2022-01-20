#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "maths.h"
#include "frame.h"
#include "texture.h"
#include "camera.h"
class Window
{
private:
	int width, height;
	static int win_width, win_height;
	static int exit;
	static Camera* camera;
	static POINT prev_pt;
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
	void set_camera(Camera* cam);
	int register_wndclass();
	int create_window(const TCHAR* title);
	int initialize_window( const TCHAR* title);	
	int create_frame(int w, int h, HWND hwnd);
	void window_messages(void);
	void window_update();
	void load_image(Texture* texture);
	
	static LRESULT CALLBACK window_procedure(HWND, UINT, WPARAM, LPARAM);
	static void handle_mouse_drag(POINT curr);
	static void handle_close_button(HWND hWnd);
	static void handle_default();
};

#endif //__WINDOW_H__