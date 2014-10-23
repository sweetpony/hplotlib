#include "Layout.hpp"

namespace hpl
{
Layout::Layout()
{
}

Layout::~Layout()
{
}

void Layout::addPlot(Geometry* plotBox)
{
    plotBoxes.push_back(plotBox);
    recalculate();
    redraw();
}

void Layout::redraw()
{
    //! @todo implement somehow
}
}
