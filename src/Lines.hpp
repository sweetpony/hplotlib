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
struct SimpleLines {
    SimpleLines(int n, double const* x, double const* y, bool separate, bool ownsX, bool ownsY) : _n(n), _x(x), _y(y), _separate(separate), _ownsX(ownsX), _ownsY(ownsY) {}
    virtual ~SimpleLines() {
        if (_ownsX) {
            delete[] _x;
        }
        if (_ownsY) {
            delete[] _y;
        }
    }

    const int _n;
    const double* _x, * _y;
    const bool _separate, _ownsX, _ownsY;
};

class Lines : public Drawable, private SimpleLines {
public:
    enum Style {
        Solid,
        Dashed,
        Dotted
    };

    Lines(int n, double const* x, double const* y, const Limits& limits, bool separate = false) :
        Drawable((separate ? Type_Lines : Type_LineStrips), limits), SimpleLines(n, x, y, separate, false, false), lines(new SimpleLines(n, x, y, separate, false, false)) {}
    virtual ~Lines() {
        delete lines;
    }

    virtual inline int n() const {
        return lines->_n;
    }

    virtual inline const double* x() const {
        return lines->_x;
    }

    virtual inline const double* y() const {
        return lines->_y;
    }

    virtual inline void setColor(const Color& c) {
        color = c;
        changed.invoke(plotId);
    }
    virtual inline Color getColor() const {
        return color;
    }

    void setThickness(double thick);
    inline double getThickness() const {
        return thickness;
    }

    void setStyle(Style s);
    inline Style getStyle() const {
        return style;
    }

    virtual void recalculateData();

protected:
    void calculateInterpolationDots(const double* thisx, const double* thisy);

    SimpleLines* lines;
    Color color = Color(0.0f, 0.0f, 0.0f);
    double thickness = 1.0;
    Style style = Solid;
};
}

#endif

