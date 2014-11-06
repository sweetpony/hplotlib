/*
 * Lines.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_LINES_HPP
#define HPLOTLIB_LINES_HPP

#include "Drawable.hpp"
#include "Statistics.hpp"

namespace hpl
{
class Lines : public Drawable {
public:
    enum Style {
        Solid,
        Dashed,
        Dotted
    };

    Lines(int n, double const* x, double const* y, bool seperate = false) : n(n), x(x), y(y), separate(seperate) {}
    virtual ~Lines() {}

    inline void setThickness(double thick) {
        thickness = thick;
        changed.invoke(plotId);
    }
    inline double getThickness() {
        return thickness;
    }

    inline void setStyle(Style s) {
        style = s;
        changed.invoke(plotId);
    }
    inline Style getStyle() {
        return style;
    }

    const int n;
    const double* x, * y;
    const bool separate;
private:
    double thickness = 1.0;
    Style style = Solid;
};
}

#endif

