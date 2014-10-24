/*
 * Window.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "../Window.hpp"
#include "Win32Window.hpp"

void* hpl::Window::run(void* self)
{
	return runProto<Win32Window>(self);
}
