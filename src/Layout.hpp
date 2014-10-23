#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <vector>
#include "Geometry.hpp"

namespace hpl
{
class Layout
{

public:
    Layout();
    virtual ~Layout();

    void addPlot(Geometry* plotBox);

    inline void setXMargins(double left, double right) {
        margins[0] = left;
        margins[1] = right;
    }

    inline void setYMargins(double top, double bottom) {
        margins[2] = top;
        margins[3] = bottom;
    }

    virtual void recalculate() = 0;

    void redraw();

protected:
    std::vector<Geometry*> plotBoxes;
    //! @todo how are margins exactly defined here?
    double margins[4]; //left, right, top, bottom

};
}

#endif // LAYOUT_HPP
