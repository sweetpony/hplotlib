/*
 * Window.cpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "../Window.hpp"
#include "X11Window.hpp"

hpl::Window::Window()
{
	base = new X11Window(this);
	pthread_create(&windowThread, nullptr, run, this);
}
