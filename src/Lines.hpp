/*
 * Lines.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_LINES_HPP
#define HPLOTLIB_LINES_HPP

#include "Plot.hpp"
#include "Statistics.hpp"

namespace hpl
{
class Lines : public Plot {
public:
    enum Style {
        Solid,
        Dashed,
        Dotted
    };

    Lines(int n, double const* x, double const* y);
    virtual ~Lines();

    inline int getN() {
        return n;
    }
    inline double getX(int i) {
        return x[i];
    }
    inline double getY(int i) {
        return y[i];
    }
	
private:
	int n;
    double const* x, * y;
    //float* interleave = nullptr;
    double xmin, ymin, xmax, ymax;
    double thickness = 1.0;
    Style style = Solid;
};
}

#endif

