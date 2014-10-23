#include "Layout.hpp"

namespace hpl
{
Layout::Layout()
{
}

Layout::~Layout()
{
}

void Layout::addPlot(Geometry* legendBox, Geometry* plotBox)
{
    plotBoxes.push_back(legendBox);
    plotBoxes.push_back(plotBox);
    recalculate();
    redraw();
}

void Layout::copyPlots(const Layout& layout)
{
    for (unsigned int i = 0; i < layout.plotBoxes.size(); i+=2) {
        addPlot(layout.plotBoxes[i], layout.plotBoxes[i+1]);
    }
}

void Layout::redraw()
{
    //! @todo implement somehow
}
}
