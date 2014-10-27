/*
 * Legend.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_LEGEND_HPP
#define HPLOTLIB_LEGEND_HPP

#include <GL/glld.h>
#include <cstdio>
#include <algorithm>

#include "PlotPart.hpp"
#include "Font.hpp"
#include "Statistics.hpp"

namespace hpl
{
class Legend : public PlotPart {
public:
	static constexpr float XOffset = 0.12f;
	static constexpr float YOffset = 0.08f;
	static constexpr int Ticks = 8;
	static constexpr float TickLength = 0.02f;

    Legend(Font* font, int n, double const* x, double const* y, Geometry* geometry);
    ~Legend();

    float* getLines() const;
    inline unsigned int getLinesCount() const {
        return 8 + 2*Ticks*4;
    }

	virtual void init(GLuint lineprogram, GLuint textprogram);
	virtual void destroy();
	virtual void draw(float const* mvp);
	
private:
	Font* font;

	double xmin;
	double xmax;
	double ymin;
	double ymax;

    GLuint lineBuffer;
    GLuint textBuffer;
    GLuint lineprogram;
    GLuint textprogram;
    
	GLint linepos, linerect, linecolor, linemvp, textpos, textuv, textrect, textglyphs, textmvp, textcolor;
	int numChars;

    float* lines = nullptr;
};
}

#endif

