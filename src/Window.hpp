/*
 * Window.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_WINDOW_HPP
#define HPLOTLIB_WINDOW_HPP

#include <pthread.h>

namespace hpl
{
	class Window {
		friend class Win32Window;
		friend class X11Window;
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
		bool loadOpenGL();
	
		pthread_t windowThread;		
		bool needsRepaint = false;
		double lastx = 0.0;
		double lasty = 0.0;
	};
}

#endif

