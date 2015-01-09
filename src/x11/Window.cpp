/*
 * Window.cpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "../Window.hpp"
#include "../GL/glld.h"
#include <iostream>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
namespace hpl
{
struct WindowPlatformStuff {
	void destroy(Window& win);
	::Window xwin;
	::Display* dpy;
	::GLXContext glc;	
	int lastx;
	int lasty;
};

Window::Window(std::string const& title)
	: title(title)
{
	platform = new WindowPlatformStuff;
	pthread_create(&windowThread, nullptr, run, this);
}

Window::~Window()
{
	delete platform;
}

bool Window::show()
{
	GLint att[] = {
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 0,
		GLX_DOUBLEBUFFER, True,
		//~ GLX_SAMPLE_BUFFERS, True,
		//~ GLX_SAMPLES, 4,
		None
	};
	
	platform->dpy = XOpenDisplay(0);
	if (platform->dpy == nullptr) {
		std::cerr << "Cannot connect to X server." << std::endl;
		return false;
	}
	::Window root = DefaultRootWindow(platform->dpy);
	
	GLXFBConfig fbconfig;
	int fbcount;
	GLXFBConfig* fbcs = glXChooseFBConfig(platform->dpy, 0, att, &fbcount);

	if (fbcs && fbcount >= 1) {
		fbconfig = fbcs[0];
	} else {
		std::cerr << "Failed to get GLXFBConfig." << std::endl;
		XCloseDisplay(platform->dpy);
		if (fbcs) {
			XFree(fbcs);			
		}
		return false;
	}
	
	XFree(fbcs);
	
	XVisualInfo *vi = glXGetVisualFromFBConfig(platform->dpy, fbconfig);

	if (vi == nullptr) {
		std::cerr << "Could not find XVisual." << std::endl;
		XCloseDisplay(platform->dpy);
		return false;
	}

	Colormap cmap = XCreateColormap(platform->dpy, root, vi->visual, AllocNone);

	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | Button1MotionMask;
	
	platform->xwin = XCreateWindow(
		platform->dpy,
		root,
		0, 0, 640, 480,
		0,
		vi->depth,
		InputOutput,
		vi->visual,
		CWColormap | CWEventMask, 
		&swa
	);
	Atom delWindow = XInternAtom(platform->dpy, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(platform->dpy, platform->xwin, &delWindow, 1);

	XMapWindow(platform->dpy, platform->xwin);
	XStoreName(platform->dpy, platform->xwin, "Canvas");
	
	platform->glc = glXCreateContext(platform->dpy, vi, 0, GL_TRUE);
	glXMakeCurrent(platform->dpy, platform->xwin, platform->glc);	

	if (!loadOpenGL()) {
		glXMakeCurrent(platform->dpy, None, NULL);
		glXDestroyContext(platform->dpy, platform->glc);
		XDestroyWindow(platform->dpy, platform->xwin);
		XCloseDisplay(platform->dpy);
		return false;
	}
	
	init();

    return true;
}

void Window::poll()
{
	int count = XPending(platform->dpy);
	while (count--) {
		XEvent xev;
		XNextEvent(platform->dpy, &xev);
				
		switch (xev.type) {
			case Expose: {
				XWindowAttributes gwa;
				XGetWindowAttributes(platform->dpy, platform->xwin, &gwa);
				glViewport(0, 0, gwa.width, gwa.height);
				width = gwa.width;
				height = gwa.height;
				update();
				break;
			}
			case KeyPress: {
				KeySym k = XLookupKeysym(&xev.xkey, 0);
				switch (k) {
					case XK_Escape:
						platform->destroy(*this);
						break;
					case XK_r:						
						resetEvent();
						update();
						break;
					default:
						break;
				}
				break;
			}
			case ButtonPress: {
				switch (xev.xbutton.button) {
					case Button1:
						lastx = xev.xbutton.x;
						lasty = xev.xbutton.y;
						break;
					case Button4:
						mouseWheelEvent(xev.xbutton.x, xev.xbutton.y, 1.0);
						update();
						break;
					case Button5:
						mouseWheelEvent(xev.xbutton.x, xev.xbutton.y, -1.0);
						update();
						break;
					default:
						break;
				}
				break;
			}
			case MotionNotify:
				if (xev.xmotion.state & Button1Mask) {
					moveEvent(xev.xmotion.x - lastx, xev.xmotion.y - lasty);
					platform->lastx = xev.xmotion.x;
					platform->lasty = xev.xmotion.y;
					update();
				}
				break;
			case ClientMessage:
				platform->destroy(*this);
				break;
			default:
				break;
		}
	}
}

void Window::repaint()
{
	draw();
	glXSwapBuffers(platform->dpy, platform->xwin); 
}

void WindowPlatformStuff::destroy(Window& win)
{
	win.destroy();
	win.isOpen = false;
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, xwin);
	XCloseDisplay(dpy);
}
}
