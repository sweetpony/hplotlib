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

namespace hpl
{
	struct WindowPlatformStuff;
	class Window {
		friend WindowPlatformStuff;
	public:
        virtual void wait() {
            needsRepaint = true;
			pthread_join(windowThread, nullptr);			
		}

        virtual void update() { needsRepaint = true; }        
		virtual void synchronise();		

	protected:
		Window(std::string const& title);
		~Window();

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
		bool show();
		void poll();
		void repaint();
	
		static double constexpr RefreshRate = 60.0;
		
		static void* run(void* self);
		bool loadOpenGL();
	
        pthread_t windowThread;
		double lastx = 0.0;
		double lasty = 0.0;
		
		WindowPlatformStuff* platform = nullptr;
		std::string title;
		bool isOpen = true;
	};
}

#endif

