#include "Lines.hpp"

namespace hpl {

Lines::Lines(int n, double const* x, double const* y, const Limits& limits, bool separate, bool ownsData) :
    Drawable((separate ? Type_Lines : Type_LineStrips), limits), SimpleLines(n, x, y, separate, ownsData, ownsData), lines(new SimpleLines(n, x, y, separate, false, false))
{
}

Lines::~Lines()
{
    delete lines;
}

void Lines::setThickness(double thick)
{
    thickness = thick;
    changed.invoke(plotId);
}

void Lines::setStyle(Style s)
{
    if (s != style) {
        style = s;
        recalculateData();
    }
}

// Assume _seperate is always false for relevant cases. Can do this as long as _seperate is a hidden property only to distinguish coordinate system and real plots
void Lines::recalculateData()
{
    delete lines;
    const double* thisx = _x,* thisy = _y;
    if (xlog) {
        thisx = log(_n, _x);
    }
    if (ylog) {
        thisy = log(_n, _y);
    }

    switch(style) {
        case Solid:
            lines = new SimpleLines(_n, thisx, thisy, _separate, xlog, ylog);
            type = (lines->_separate ? Type_Lines : Type_LineStrips);
            break;
        case Dashed:
            //! @todo perhaps want to improve this such that beginning and end are connected by visible lines no matter if n % 2 is 0 or 1
            lines = new SimpleLines(_n, thisx, thisy, !_separate, xlog, ylog);
            type = (lines->_separate ? Type_Lines : Type_LineStrips);
            break;
        case Dotted:
            calculateInterpolationDots(thisx, thisy);
            type = Type_Points;
            break;
    }

    changed.invoke(plotId);
}

void Lines::calculateInterpolationDots(const double* thisx, const double* thisy)
{
    unsigned int n = 300;
    double dx = limits.xmax() - limits.xmin();
    double dy = limits.ymax() - limits.ymin();

    //! @todo Actually would need aspect ratio of current box, which is geometry & aspect ratio of window
    double fx = (thisx[_n-1] - thisx[0]) / dx;
    double fy = (max(_n, thisy) - min(_n, thisy)) / dy;
    double f = std::min(fx, fy);
    if (f < 1.0) {
        n *= f;
    }

    dx /= n;
    dy /= n;

    double* x = new double[n];
    double* y = new double[n];

    unsigned int j = 0;
    while (thisx[j+1] < limits.xmin()) {
        ++j;
    }

    for (unsigned int i = 0; i < n; i++) {
        x[i] = (i == 0 ? limits.xmin() : x[i-1]) + dx;

        while (x[i] > thisx[j+1]) {
            j++;
        }
        if (j+1 < static_cast<unsigned int>(_n)) {
            y[i] = interpolate(x[i], thisx[j], thisy[j], thisx[j+1], thisy[j+1]);
        } else {
            y[i] = interpolate(x[i], thisx[j], thisy[j], thisx[j], thisy[j]);
        }
    }

    lines = new SimpleLines(n, x, y, true, true, true);

    if (xlog) {
        delete[] thisx;
    }
    if (ylog) {
        delete[] thisy;
    }
}
}
