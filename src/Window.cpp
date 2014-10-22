/*
 * Window.cpp
 *
 *  Created on: Oct 23, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "Window.hpp"

#include <GL/glld.h>
#include <iostream>

bool hpl::Window::loadOpenGL()
{
	bool success = (ogl_LoadFunctions() != ogl_LOAD_FAILED);
	if (!success) {
		std::cerr << "Failed to initialise OpenGL." << std::endl;
	}
	return success;
}
