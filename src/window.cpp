#include "window.h"
#include <iostream>

int Window::win_width = 0;
int Window::win_height = 0;
LPCSTR class_name = "Main_Window_Class";
int Window::exit = 0;
HWND Window::hwnd = NULL; //windle handle
HDC Window::hdc = NULL;	//a handle to something you can draw on
HBITMAP Window::bi_handle =NULL;		
HBITMAP Window::old_bi_handle = NULL;		
Camera* Window::camera = NULL;
POINT Window::prev_pt;
Window::Window(int w, int h) :width(w), height(h)
{
	frame = new Frame(width, height);

}
Window::Window(Frame* f) : frame(f)
{
	width = f->get_width();
	height = f->get_height();
}
Window::~Window() 
{

	delete frame;
	if (hdc) {
		if (old_bi_handle)
		{
			SelectObject(hdc, old_bi_handle);
			DeleteObject(old_bi_handle);
			
		}
		if (bi_handle)
		{
			DeleteObject(bi_handle);
		}
		DeleteDC(hdc);
	}
	if (hwnd)
	{
		CloseWindow(hwnd);
	}
	
};



void Window::load_image(Texture* texture)
{
	if (texture)
	{
		unsigned int* img = (unsigned int*)texture->get_texture();
		int img_width = texture->get_width();
		int img_height = texture->get_height();
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				
				if (i<img_height && j<img_width)
				{
					buffer[i * width + j] = img[i * img_width + j];
					
				}
				else 
				{
					buffer[i * width + j] = 0;
				}
			}
		}
	}
}

int Window::get_window_exit()
{
	return exit;
}

void Window::set_camera(Camera* cam)
{
	camera = cam;
}


//register the window class
int Window::register_wndclass()
{	
	WNDCLASS wc = {};
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_procedure;
	wc.lpszClassName =class_name;
	if (!RegisterClass(&wc)) return -1;

	return 0;
}
//create the window
int Window::create_window( const TCHAR* title)
{
	DWORD window_style =  WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		class_name,
		title,
		window_style,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, 
		NULL,
		GetModuleHandle(NULL),
		NULL);
	if (hwnd == NULL) return -1;
	exit = 0;
	SetForegroundWindow(hwnd);
	create_frame(width, height, hwnd);

	ShowWindow(hwnd, SW_NORMAL);
	window_messages();
	win_width = width;
	win_height = height;
	return 0;
}

int Window::initialize_window(const TCHAR* title) {
	if (register_wndclass())
		return -1;
	
	if (create_window( title))
		return -1;
	return 0;
}

int Window::create_frame(int w, int h, HWND hwnd)
{
	//if bheight is positive, bitmap is bottom-up dib (device independent bitmap)
	//if bheight is negative, bit map is top-down dib
	BITMAPINFOHEADER bi_header{
		sizeof(BITMAPINFOHEADER),
		w, h,
		1, 32, BI_RGB,
		w * h * 4,
		0, 0, 0, 0
	};
	//bit map info contain color 
	BITMAPINFO bi_info
	{ 
		bi_header
	};
	LPVOID bi_ptr;//pointer to a variable that receives a pointer to the location of the DIB bit values
	HDC window_dc = GetDC(hwnd);//dc from the window
	hdc = CreateCompatibleDC(window_dc);//compatible dc
	ReleaseDC(hwnd, window_dc);

	//create dcbitmap that can write to directly.
	//return a handle to a bitmap
	bi_handle = CreateDIBSection(
		hdc,
		&bi_info,//a pointer to bitmapinfo structure
		DIB_RGB_COLORS,
		&bi_ptr,
		0,
		0);
	
	if (bi_handle == NULL) return -1;
	old_bi_handle = (HBITMAP)SelectObject(hdc, bi_handle);//save old handle for deleting/releasing m
	
	buffer = (unsigned int*)bi_ptr;//buffer is assigned to the new created bitmap
	memset(buffer, 0, w * h);
	frame->init_framebuffer(buffer);

}


//defines the behavior of the window
LRESULT CALLBACK Window::window_procedure(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
		case WM_LBUTTONDOWN:
		{
			prev_pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			break;
		}
		case WM_MOUSEMOVE:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			if (DragDetect(hWnd, pt))
			{
				handle_mouse_drag(pt);
			}
			break;
		}
		case WM_CLOSE:
		{
			handle_close_button(hWnd);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	return 0;
}

void Window::handle_mouse_drag(POINT curr_pt)
{

	float2 dist = float2(2*(prev_pt.x- curr_pt.x)/(float)win_width , 2 * (prev_pt.y - curr_pt.y) / (float)win_height);
	camera->orbit_around_target(dist);
	prev_pt = curr_pt;
}

void Window::handle_close_button(HWND hWnd)
{
	exit = 1;
	if (MessageBox(hWnd, "Really quit?", "Renderer", MB_OKCANCEL) == IDOK)
	{
		DestroyWindow(hWnd);
	}
}



void Window::window_messages(void) {
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::window_update() {
	HDC dst_hdc = GetDC(hwnd);
	//bit-block transfer of the color data corresponding to a rectagle of pixels from the specified source dc into a destination dc
	//orginal hdc's buffer pointing to framebuffer
	BitBlt(dst_hdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, dst_hdc);
	window_messages();
}

