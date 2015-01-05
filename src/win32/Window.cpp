/*
 * Window.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "../Window.hpp"
#include "Win32Window.hpp"

#include <iostream>

hpl::Window::Window()
{
	base = new Win32Window(this);
	pthread_create(&windowThread, nullptr, run, this);
}
