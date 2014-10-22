/*
 * Lines.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef PLOT_LINES_HPP
#define PLOT_LINES_HPP

#include <types.hpp>
#include "Plot.hpp"

namespace plot
{
class Lines : public Plot {
public:
	Lines(Uint n, Doub const* x, Doub const* y, float r, float g, float b, float left, float top, float width, float height);

	virtual void init(GLuint lineprogram, GLuint);
	virtual void destroy();
	virtual void draw(float const* mvp);
	
private:
	Uint n;
	float* interleave = nullptr;

    GLuint lineBuffer;
    GLuint program;
    
    GLint pos, rect, color, linemvp;
    float r, g, b;
};
}

#endif

