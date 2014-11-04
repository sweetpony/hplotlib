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

    void resetData(int n, double const* x, double const* y, bool del);

    inline void setThickness(double thick) {
        thickness = thick;
        changed.invoke();
    }
    inline double getThickness() {
        return thickness;
    }

    inline void setStyle(Style s) {
        style = s;
        changed.invoke();
    }
    inline Style getStyle() {
        return style;
    }

private:
    int n;
    double const* x, * y; //! @todo for some need to delete these afterwards, handle this somehow
    double thickness = 1.0;
    Style style = Solid;
};
}

#endif

