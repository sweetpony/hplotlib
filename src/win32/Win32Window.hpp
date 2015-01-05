/*
 * Win32Window.hpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_WIN32WINDOW_HPP
#define HPLOTLIB_WIN32WINDOW_HPP

#include "../Window.hpp"
#include "../WindowBase.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace hpl
{
	class Win32Window : public WindowBase {
	public:
		Win32Window(Window* win) : WindowBase(win) {}
		virtual bool show();
		virtual void poll();
		virtual void update();
	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		HGLRC glcontext;
		HWND hWnd;
	};
}

#endif

