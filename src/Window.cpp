/*
 * Window.cpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "Window.hpp"

#include "GL/glld.h"
#include <iostream>

bool hpl::Window::loadOpenGL()
{
	bool success = (ogl_LoadFunctions() != ogl_LOAD_FAILED);
	if (!success) {
		std::cerr << "Failed to initialise OpenGL." << std::endl;
	}
	return success;
}

void hpl::Window::synchronise()
{
	pthread_mutex_lock(&mutex);
	needsSynchronise = true;
	while (pthread_cond_wait(&synCond, &mutex));
	pthread_mutex_unlock(&mutex);
}

void* hpl::Window::run(void* self)
{
	Window* win = static_cast<Window*>(self);
	win->base->show();

	auto last = std::chrono::steady_clock::now();
	while (win->isOpen) {
		win->base->poll();
		
		pthread_mutex_lock(&win->mutex);
		if (win->needsSynchronise) {
			win->processDrawables();
			win->needsSynchronise = false;
			pthread_cond_signal(&win->synCond);
		}
		pthread_mutex_unlock(&win->mutex);
		if (win->needsRepaint) {
			win->base->update();
			win->needsRepaint = false;
		}

		std::chrono::microseconds timeSpan = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - last);
		sleep(std::round(std::max(0.0, 1e6 / RefreshRate - timeSpan.count())));
		last = std::chrono::steady_clock::now();
	}

	return nullptr;
}
