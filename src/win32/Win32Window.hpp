/*
 * Win32Window.hpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_WIN32WINDOW_HPP
#define HPLOTLIB_WIN32WINDOW_HPP

#include "../Window.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace hpl
{
	class Win32Window {
	public:
		bool show(Window* windowBase);
		void poll();
		void update();
	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		HGLRC glcontext;
		HWND hWnd;
		Window* win;
	};
}

#endif

