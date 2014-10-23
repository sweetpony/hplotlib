/*
 * Plot.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_PLOTPART_HPP
#define HPLOTLIB_PLOTPART_HPP

#include <GL/glld.h>

namespace hpl
{
class PlotPart {
public:
    PlotPart(float left, float top, float width, float height) : left(left), top(top), width(width), height(height) {}
    virtual ~PlotPart() {}

	virtual void init(GLuint lineprogram, GLuint textprogram) = 0;
	virtual void destroy() = 0;
	virtual void draw(float const* mvp) = 0;
	
protected:
	float left, top, width, height;
};
}

#endif

