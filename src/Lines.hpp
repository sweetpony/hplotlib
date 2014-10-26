/*
 * Lines.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_LINES_HPP
#define HPLOTLIB_LINES_HPP

#include "PlotPart.hpp"
#include "Statistics.hpp"

namespace hpl
{
class Lines : public PlotPart {
public:
    Lines(int n, double const* x, double const* y, Geometry* geometry);
    virtual ~Lines();

    float* getX() const;
    float* getY() const;

    inline int getN() const {
        return n;
    }

    inline double getXMin() const {
        return xmin;
    }
    inline double getYMin() const {
        return ymin;
    }
    inline double getXMax() const {
        return xmax;
    }
    inline double getYMax() const {
        return ymax;
    }

	virtual void init(GLuint lineprogram, GLuint);
	virtual void destroy();
	virtual void draw(float const* mvp);
	
private:
	int n;
	float* interleave = nullptr;
    double xmin, ymin, xmax, ymax;

    GLuint lineBuffer;
    GLuint program;
    
    GLint pos, rect, color, linemvp;
};
}

#endif

