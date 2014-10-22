/*
 * Canvas.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef PLOT_WINDOW_HPP
#define PLOT_WINDOW_HPP

#include <pthread.h>

namespace plot
{
	class Window {
	public:
		void wait() {
			pthread_join(windowThread, nullptr);			
		}
	protected:
		virtual void init() = 0;
		virtual void destroy() = 0;
		virtual void draw() = 0;
		virtual void resetEvent() = 0;
		virtual void moveEvent(double deltax, double deltay) = 0;
		virtual void mouseWheelEvent(double x, double y, double delta) = 0;
		
		Window() { pthread_create(&windowThread, nullptr, run, this); }
		static void* run(void* self);
		
		double width = 0.0;
		double height = 0.0;
		
	private:
		pthread_t windowThread;
		
		#if defined(_WIN32)
			static VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			HGLRC glcontext;
		#endif
		
		bool needsRepaint = false;
		double lastx = 0.0;
		double lasty = 0.0;
	};
}

#endif

