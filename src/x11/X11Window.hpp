/*
 * X11Window.hpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_X11WINDOW_HPP
#define HPLOTLIB_X11WINDOW_HPP

#include "../Window.hpp"
#include <GL/glld.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>

namespace hpl
{
	class X11Window {
	public:
		bool show(Window* win);
		void poll();
		void update();
		
	private:
		void close();

		Window* win;
		::Window xwin;
		::Display* dpy;
		::GLXContext glc;
		
		int lastx;
		int lasty;
	};
}

#endif

