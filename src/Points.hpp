#ifndef POINTS_H
#define POINTS_H

#include "Drawable.hpp"

namespace hpl
{
class Points : public Drawable
{

public:
    enum Style {
        Dot,
        Asterisk,
        Plus,
        Circle,
        FilledCircle,
        Cross
    };

    Points(int n, double const* x, double const* y) : Drawable(), n(n), x(x), y(y){}
    virtual ~Points() {}

    virtual inline void setColor(const Color& c) {
        color = c;
        changed.invoke(plotId);
    }
    virtual inline Color getColor() const {
        return color;
    }

    inline void setSymbolSize(double thick) {
        size = thick;
        changed.invoke(plotId);
    }
    inline double getSymbolSize() const {
        return size;
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

protected:
    Color color = Color(0.0f, 0.0f, 0.0f);
    double size = 1.0;
    Style style = Dot;

};
}

#endif // POINTS_H
