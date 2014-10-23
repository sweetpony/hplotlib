/*
 * Window.cpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "../Window.hpp"
#include "X11Window.hpp"

void* hpl::Window::run(void* self)
{
	X11Window window;
	window.show(static_cast<Window*>(self));
	return nullptr;
}
