#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <vector>
#include "Geometry.hpp"
#include "Legend.hpp"
#include "Delegate.hpp"

namespace hpl
{
class Layout
{

public:
    Layout();
    virtual ~Layout();

    void addPlot(Geometry* legendBox, Geometry* plotBox);
    void copyPlots(const Layout& layout);

    inline void setXMargins(double left, double right) {
        margins[0] = left;
        margins[1] = right;
    }

    inline void setYMargins(double top, double bottom) {
        margins[2] = top;
        margins[3] = bottom;
    }

    virtual void recalculate() = 0;

    Delegate<> changed;

protected:
    std::vector<Geometry*> plotBoxes;
    //! @todo how are margins exactly defined here? use them
    double margins[4]; //left, right, top, bottom

};
}

#endif // LAYOUT_HPP
