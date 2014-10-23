/*
 * X11Window.cpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "X11Window.hpp"

#include <GL/glld.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <iostream>

bool hpl::X11Window::show(hpl::Window* win)
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
	
	Display* dpy = XOpenDisplay(0);
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
	swa.event_mask = ExposureMask | KeyPressMask;
	
	::Window xwin = XCreateWindow(
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
	
	GLXContext glc = glXCreateContext(dpy, vi, 0, GL_TRUE);
	glXMakeCurrent(dpy, xwin, glc);	

	if (!win->loadOpenGL()) {
		glXMakeCurrent(dpy, None, NULL);
		glXDestroyContext(dpy, glc);
		XDestroyWindow(dpy, xwin);
		XCloseDisplay(dpy);
		return false;
	}
	
	win->init();
	
	bool close = false;
	while (!close) {
		XEvent xev;
		if (win->updateRequired()) {
			win->updateQueued();
			win->draw();
			glXSwapBuffers(dpy, xwin);
		} else {
			XNextEvent(dpy, &xev);
			
			switch (xev.type) {
				case Expose: {
					XWindowAttributes gwa;
					XGetWindowAttributes(dpy, xwin, &gwa);
					glViewport(0, 0, gwa.width, gwa.height);
					win->update();
					break;
				}
				case KeyPress: {
					KeySym k = XLookupKeysym(&xev.xkey, 0);
					switch (k) {
						case XK_Escape:
							close = true;
							break;
						case XK_r:						
							win->resetEvent();
							win->update();
							break;
						case XK_Left:
							win->moveEvent(0.1f, 0.0f);
							win->update();
							break;
						case XK_Right:
							win->moveEvent(-0.1f, 0.0f);
							win->update();
							break;
						case XK_Up:
							win->moveEvent(0.0f, -0.1f);
							win->update();
							break;
						case XK_Down:
							win->moveEvent(0.0f, 0.1f);
							win->update();
							break;
						default:
							break;
					}
					break;
				}
				case ClientMessage:
					close = true;
					break;
				default:
					break;
			}
		}
    }
    
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, xwin);
	XCloseDisplay(dpy);

    return true;
}
