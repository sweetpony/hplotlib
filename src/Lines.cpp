#include "Lines.hpp"

namespace hpl {
void Lines::setThickness(double thick)
{
    thickness = thick;
    changed.invoke(plotId);
}

// Assume _seperate is always false for relevant cases. Can do this as long as _seperate is a hidden property only to distinguish coordinate system and real plots
void Lines::setStyle(Style s)
{
    if (s != style) {
        delete lines;

        switch(s) {
            case Solid:
                lines = new SimpleLines(_n, _x, _y, _separate, false);
                type = (lines->_separate ? Type_Lines : Type_LineStrips);
                break;
            case Dashed:
                //! @todo perhaps want to improve this such that beginning and end are connected by visible lines no matter if n % 2 is 0 or 1
                lines = new SimpleLines(_n, _x, _y, !_separate, false);
                type = (lines->_separate ? Type_Lines : Type_LineStrips);
                break;
            case Dotted:
                calculateInterpolationDots();
                type = Type_Points;
                break;
        }

        style = s;
        changed.invoke(plotId);
    }
}

void Lines::calculateInterpolationDots()
{
    unsigned int n = 300;
    double dx = xmax - xmin;
    double dy = ymax - ymin;

    double fx = (_x[_n-1] - _x[0]) / dx;
    double fy = (max(_n, _y) - min(_n, _y)) / dy;
    double f = std::min(fx, fy);
    if (f < 1.0) {
        n *= f;
    }

    dx /= n;
    dy /= n;

    double* x = new double[n];
    double* y = new double[n];

    unsigned int j = 0;
    while (_x[j+1] < xmin) {
        ++j;
    }

    for (unsigned int i = 0; i < n; i++) {
        x[i] = (i == 0 ? xmin : x[i-1]) + dx;

        while (x[i] > _x[j+1]) {
            j++;
        }
        if (j+1 < static_cast<unsigned int>(_n)) {
            y[i] = interpolate(x[i], _x[j], _y[j], _x[j+1], _y[j+1]);
        } else {
            y[i] = interpolate(x[i], _x[j], _y[j], _x[j], _y[j]);
        }
    }

    lines = new SimpleLines(n, x, y, true, true);
}
}
