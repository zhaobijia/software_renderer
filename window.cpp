#include "window.h"
#include <iostream>

LPCSTR class_name = "Main_Window_Class";
int Window::exit = 0;
HWND Window::hwnd = NULL; //windle handle
HDC Window::hdc = NULL;	//a handle to something you can draw on
HBITMAP Window::bi_handle =NULL;		
HBITMAP Window::old_bi_handle = NULL;		

Window::Window(int w, int h) :width(w), height(h)
{
	buffer = new unsigned int[w * h]{};
	frame = new Frame(w, h);
	
}

Window::~Window() 
{
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

void Window::set_pixel(int x, int y, Color color)
{
	frame->set_pixel(x, y, color);
}

int Window::get_window_exit()
{
	return exit;
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
int Window::create_window(int w, int h, const TCHAR* title)
{
	DWORD window_style =  WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		class_name,
		title,
		window_style,
		CW_USEDEFAULT, CW_USEDEFAULT, w, h,
		NULL, 
		NULL,
		GetModuleHandle(NULL),
		NULL);
	if (hwnd == NULL) return -1;
	exit = 0;
	SetForegroundWindow(hwnd);
	create_frame(w, h, hwnd);
	ShowWindow(hwnd, SW_NORMAL);
	window_messages();

	return 0;
}

int Window::initialize_window(int w, int h, const TCHAR* title) {
	if (register_wndclass())
		return -1;
	
	if (create_window(w, h, title))
		return -1;
	return 0;
}

int Window::create_frame(int w, int h, HWND hwnd)
{

	BITMAPINFOHEADER bi_header{
		sizeof(BITMAPINFOHEADER),
		w, -h,
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
	frame->set_framebuffer(buffer);
}


//defines the behavior of the window
LRESULT CALLBACK Window::window_procedure(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
		case WM_CLOSE:
			exit = 1; 
			if (MessageBox(hwnd, "Really quit?", "Renderer", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hwnd);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		//add mouse clicking event here
		default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
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

