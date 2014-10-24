/*
 * Window.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_WINDOW_HPP
#define HPLOTLIB_WINDOW_HPP

#include <pthread.h>
#include <chrono>
#include <algorithm>

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
		Window() { pthread_create(&windowThread, nullptr, run, this); }

		virtual void init() = 0;
		virtual void destroy() = 0;
		virtual void draw() = 0;
		virtual void resetEvent() = 0;
		virtual void moveEvent(double deltax, double deltay) = 0;
		virtual void mouseWheelEvent(double x, double y, double delta) = 0;
		
		double width = 0.0;
		double height = 0.0;
		
	private:	
		static double constexpr RefreshRate = 60.0;
		
		static void* run(void* self);
		template<class T>
		static void* runProto(void* self);
		bool loadOpenGL();
		void update() { needsRepaint = true; }
		void close() { destroy(); isOpen = false; }
	
		pthread_t windowThread;		
		bool needsRepaint = false;
		bool isOpen = true;
		double lastx = 0.0;
		double lasty = 0.0;
	};
}

template<class T>
void* hpl::Window::runProto(void* self)
{
	Window* win = static_cast<Window*>(self);
	T window;
	window.show(win);

	auto last = std::chrono::steady_clock::now();
	while (win->isOpen) {
		window.poll();

		if (win->needsRepaint) {
			window.update();
			win->needsRepaint = false;
		}

		std::chrono::milliseconds timeSpan = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last);
		Sleep(std::round(std::max(0.0, 1e3 / RefreshRate - timeSpan.count())));
		last = std::chrono::steady_clock::now();
	}

	return nullptr;
}


#endif

