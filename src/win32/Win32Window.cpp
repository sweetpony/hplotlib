/*
 * Win32Window.cpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "Win32Window.hpp"

#include <GL/glld.h>
#include <windowsx.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
typedef HGLRC (WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);

#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_SAMPLE_BUFFERS_ARB 0x2041
#define WGL_SAMPLES_ARB 0x2042
typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

namespace hpl {
LRESULT CALLBACK Win32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Win32Window* win32 = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
    switch (message) {
        case WM_CREATE: {
            LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
            break;
        }
        case WM_SIZE: {
            HDC hdc = GetDC(hWnd);
			wglMakeCurrent(hdc, win32->glcontext);
            glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
            win32->win->width = LOWORD(lParam);
            win32->win->height = HIWORD(lParam);
            break;
		}
        case WM_PAINT: {
            HDC hdc = GetDC(hWnd);
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			wglMakeCurrent(hdc, win32->glcontext);
            win32->win->draw();
            SwapBuffers(GetDC(hWnd));
			EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY: {
            HDC hdc = GetDC(hWnd);
			wglMakeCurrent(hdc, win32->glcontext);
			win32->win->close();
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(win32->glcontext);
            ReleaseDC(hWnd, GetDC(hWnd));
            PostQuitMessage(0);
            break;
        }
        case WM_KEYDOWN: {
            switch (wParam) {
                case VK_ESCAPE:
                    PostMessage(hWnd, WM_DESTROY, NULL, NULL);
                    break;
				case 0x52: // R
					win32->win->resetEvent();
					win32->win->update();
					break;
				case VK_LEFT:
					win32->win->moveEvent(0.1f, 0.0f);
					win32->win->update();
					break;
				case VK_RIGHT:
					win32->win->moveEvent(-0.1f, 0.0f);
					win32->win->update();
					break;
				case VK_UP:
					win32->win->moveEvent(0.0f, -0.1f);
					win32->win->update();
					break;
				case VK_DOWN:
					win32->win->moveEvent(0.0f, 0.1f);
					win32->win->update();
					break;
				default:
					break;
            }
            break;
		}
		case WM_LBUTTONDOWN: {
			win32->win->lastx = GET_X_LPARAM(lParam);
			win32->win->lasty = GET_Y_LPARAM(lParam);
			break;
		}
		case WM_MOUSEMOVE: {
			if (wParam == MK_LBUTTON) {
				double x = GET_X_LPARAM(lParam);
				double y = GET_Y_LPARAM(lParam);
				win32->win->moveEvent((x - win32->win->lastx) / win32->win->width, -(y - win32->win->lasty) / win32->win->height);
				win32->win->update();
				win32->win->lastx = x;
				win32->win->lasty = y;
			}
			break;
		}
		case WM_MOUSEWHEEL: {
            POINT p;
            p.x = GET_X_LPARAM(lParam);
            p.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &p);
			win32->win->mouseWheelEvent(
				p.x,
				p.y,
				GET_WHEEL_DELTA_WPARAM(wParam) / static_cast<double>(WHEEL_DELTA)
			);
			win32->win->update();
			break;
		}
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
    }

    return 0;
}

bool Win32Window::show(Window* windowBase)
{
	win = windowBase;

    char const windowClass[] = "canvas";

    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wc;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = sizeof(Win32Window*);
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, IDI_WINLOGO);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = windowClass;
    wc.hIconSm       = LoadIcon(hInstance, IDI_WINLOGO);

    RegisterClassEx(&wc);

    HWND hWndTmp = CreateWindow(windowClass, "Canvas", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);
    
    // Create a basic OpenGL context
	PIXELFORMATDESCRIPTOR  pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0,
		24,
		0, 0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};	

	HDC hdcTmp = GetDC(hWndTmp);
	int pf = ChoosePixelFormat(hdcTmp, &pfd);
	SetPixelFormat(hdcTmp, pf, &pfd);
	glcontext = wglCreateContext(hdcTmp);
	wglMakeCurrent(hdcTmp, glcontext);	
	
	auto wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
	auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
	int iAttribs[] = {
		WGL_SAMPLES_ARB, 4,
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 0,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		0
	};
	float fAttribs[] = {0};
	int pixelFormat;
	UINT numFormats;
	
	GLint attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	DestroyWindow(hWndTmp);
    hWnd = CreateWindow(windowClass, "Canvas", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, this);
    HDC hdc = GetDC(hWnd);
	int msaa = 16;
	while (msaa >= 0 && !wglChoosePixelFormatARB(hdc, iAttribs, fAttribs, 1, &pixelFormat, &numFormats)) {
		--msaa;
	}
	SetPixelFormat(hdc, pixelFormat, &pfd);
	glcontext = wglCreateContextAttribsARB(hdc, 0, attribs);
	wglMakeCurrent(hdc, glcontext);

	if (!win->loadOpenGL()) {
		return false;
	}
	
	win->init();
    
    ShowWindow(hWnd, SW_MAXIMIZE);
    UpdateWindow(hWnd);

    return true;
}

void Win32Window::poll()
{
	MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Win32Window::update()
{
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
}
}
