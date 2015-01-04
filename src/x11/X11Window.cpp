/*
 * X11Window.cpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "X11Window.hpp"

#include <iostream>

bool hpl::X11Window::show()
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
	
	dpy = XOpenDisplay(0);
	if (dpy == nullptr) {
		std::cerr << "Cannot connect to X server." << std::endl;
		return false;
	}
	::Window root = DefaultRootWindow(dpy);
	
	GLXFBConfig fbconfig;
	int fbcount;
	GLXFBConfig* fbcs = glXChooseFBConfig(dpy, 0, att, &fbcount);

	if (fbcs && fbcount >= 1) {
		fbconfig = fbcs[0];
	} else {
		std::cerr << "Failed to get GLXFBConfig." << std::endl;
		XCloseDisplay(dpy);
		if (fbcs) {
			XFree(fbcs);			
		}
		return false;
	}
	
	XFree(fbcs);
	
	XVisualInfo *vi = glXGetVisualFromFBConfig(dpy, fbconfig);

	if (vi == nullptr) {
		std::cerr << "Could not find XVisual." << std::endl;
		XCloseDisplay(dpy);
		return false;
	}

	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | Button1MotionMask;
	
	xwin = XCreateWindow(
		dpy,
		root,
		0, 0, 640, 480,
		0,
		vi->depth,
		InputOutput,
		vi->visual,
		CWColormap | CWEventMask, 
		&swa
	);
	Atom delWindow = XInternAtom(dpy, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(dpy, xwin, &delWindow, 1);

	XMapWindow(dpy, xwin);
	XStoreName(dpy, xwin, "Canvas");
	
	glc = glXCreateContext(dpy, vi, 0, GL_TRUE);
	glXMakeCurrent(dpy, xwin, glc);	

	if (!win->loadOpenGL()) {
		glXMakeCurrent(dpy, None, NULL);
		glXDestroyContext(dpy, glc);
		XDestroyWindow(dpy, xwin);
		XCloseDisplay(dpy);
		return false;
	}
	
	win->init();

    return true;
}

void hpl::X11Window::poll()
{
	int count = XPending(dpy);
	while (count--) {
		XEvent xev;
		XNextEvent(dpy, &xev);
				
		switch (xev.type) {
			case Expose: {
				XWindowAttributes gwa;
				XGetWindowAttributes(dpy, xwin, &gwa);
				glViewport(0, 0, gwa.width, gwa.height);
				win->width = gwa.width;
				win->height = gwa.height;
				win->update();
				break;
			}
			case KeyPress: {
				KeySym k = XLookupKeysym(&xev.xkey, 0);
				switch (k) {
					case XK_Escape:
						close();
						break;
					case XK_r:						
						win->resetEvent();
						win->update();
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
						win->mouseWheelEvent(xev.xbutton.x, xev.xbutton.y, 1.0);
						win->update();
						break;
					case Button5:
						win->mouseWheelEvent(xev.xbutton.x, xev.xbutton.y, -1.0);
						win->update();
						break;
					default:
						break;
				}
				break;
			}
			case MotionNotify:
				if (xev.xmotion.state & Button1Mask) {
					win->moveEvent(xev.xmotion.x - lastx, xev.xmotion.y - lasty);
					lastx = xev.xmotion.x;
					lasty = xev.xmotion.y;
					win->update();
				}
				break;
			case ClientMessage:
				close();
				break;
			default:
				break;
		}
	}
}

void hpl::X11Window::update()
{
	win->draw();
	glXSwapBuffers(dpy, xwin); 
}

void hpl::X11Window::close()
{
	win->close();
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, xwin);
	XCloseDisplay(dpy);
}
