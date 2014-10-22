/*
 * Legend.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef PLOT_LEGEND_HPP
#define PLOT_LEGEND_HPP

#include <types.hpp>
#include <GL/glld.h>

#include "Plot.hpp"
#include "Font.hpp"

namespace plot
{
class Legend : public Plot {
public:
	static constexpr float XOffset = 0.12f;
	static constexpr float YOffset = 0.08f;
	static constexpr int Ticks = 8;
	static constexpr float TickLength = 0.02f;
	Legend(Font* font, Uint n, Doub const* x, Doub const* y, float left, float top, float width, float height);

	virtual void init(GLuint lineprogram, GLuint textprogram);
	virtual void destroy();
	virtual void draw(float const* mvp);
	
private:
	Font* font;

	Doub xmin;
	Doub xmax;
	Doub ymin;
	Doub ymax;

    GLuint lineBuffer;
    GLuint textBuffer;
    GLuint lineprogram;
    GLuint textprogram;
    
	GLint linepos, linerect, linecolor, linemvp, textpos, textuv, textrect, textglyphs, textmvp;
	int numChars;
};
}

#endif

