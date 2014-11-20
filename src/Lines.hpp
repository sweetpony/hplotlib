/*
 * Lines.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_LINES_HPP
#define HPLOTLIB_LINES_HPP

#include "Drawable.hpp"

namespace hpl
{
class Lines : public Drawable {
public:
    enum Style {
        Solid,
        Dashed,
        Dotted
    };

    Lines(int n, double const* x, double const* y, bool seperate = false) : Drawable(), n(n), x(x), y(y), separate(seperate) {}
    virtual ~Lines() {}

    virtual inline void setColor(const Color& c) {
        color = c;
        changed.invoke(plotId);
    }
    virtual inline Color getColor() const {
        return color;
    }

    inline void setThickness(double thick) {
        thickness = thick;
        changed.invoke(plotId);
    }
    inline double getThickness() const {
        return thickness;
    }

    inline void setStyle(Style s) {
        style = s;
        changed.invoke(plotId);
    }
    inline Style getStyle() const {
        return style;
    }

    const int n;
    const double* x, * y;
    const bool separate;

protected:
    Color color = Color(0.0f, 0.0f, 0.0f);
    double thickness = 1.0;
    Style style = Solid;
};
}

#endif

