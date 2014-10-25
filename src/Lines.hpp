/*
 * Lines.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_LINES_HPP
#define HPLOTLIB_LINES_HPP

#include "PlotPart.hpp"

namespace hpl
{
class Lines : public PlotPart {
public:
    Lines(int n, double const* x, double const* y, Geometry* geometry);
    virtual ~Lines();

	virtual void init(GLuint lineprogram, GLuint);
	virtual void destroy();
	virtual void draw(float const* mvp);
	
private:
	int n;
	float* interleave = nullptr;

    GLuint lineBuffer;
    GLuint program;
    
    GLint pos, rect, color, linemvp;
};
}

#endif

