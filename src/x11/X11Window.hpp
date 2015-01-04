/*
 * X11Window.hpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_X11WINDOW_HPP
#define HPLOTLIB_X11WINDOW_HPP

#include "../Window.hpp"
#include "../WindowBase.hpp"
#include "../GL/glld.h"
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>

namespace hpl
{
	class X11Window : public WindowBase {
	public:
		X11Window(Window* win) : WindowBase(win) {}
		virtual bool show();
		virtual void poll();
		virtual void update();
		
	private:
		void close();

		::Window xwin;
		::Display* dpy;
		::GLXContext glc;
		
		int lastx;
		int lasty;
	};
}

#endif

