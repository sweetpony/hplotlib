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

#include "Sleep.hpp"
#include "WindowBase.hpp"

namespace hpl
{	
	class Window {
		friend class Win32Window;
		friend class X11Window;
	public:
        void wait() {
            needsRepaint = true;
			pthread_join(windowThread, nullptr);			
		}

        virtual void update() { needsRepaint = true; }
        
		virtual void synchronise();

	protected:
		Window();
		~Window() { delete base; }

		virtual void init() = 0;
		virtual void destroy() = 0;
		virtual void draw() = 0;
		virtual void processDrawables() = 0;
		virtual void resetEvent() = 0;
		virtual void moveEvent(int deltax, int deltay) = 0;
		virtual void mouseWheelEvent(int x, int y, double delta) = 0;
		
		double width = 0.0;
		double height = 0.0;
        bool needsRepaint = false;
        bool needsSynchronise = false;

		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		pthread_cond_t synCond = PTHREAD_COND_INITIALIZER;
		
	private:	
		static double constexpr RefreshRate = 60.0;
		
		static void* run(void* winBase);
		bool loadOpenGL();
		void close() { destroy(); isOpen = false; }
	
        pthread_t windowThread;
		bool isOpen = true;
		double lastx = 0.0;
		double lasty = 0.0;
		
		WindowBase* base = nullptr;
	};
}

#endif

