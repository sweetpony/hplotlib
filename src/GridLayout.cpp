#include "GridLayout.hpp"

namespace hpl
{
GridLayout::GridLayout(unsigned int xCount, unsigned int yCount, Orientation orientation) : Layout(), xCount(xCount), yCount(yCount), orientation(orientation)
{
}

GridLayout::~GridLayout()
{
}

void GridLayout::recalculate()
{
    unsigned int c = plotBoxes.size();
    if (c > 2 * xCount * yCount) {
        c = 2 * xCount * yCount;
    }
    double w = 1.0 / (xCount);
    double h = 1.0 / (yCount);

    double l = 0.0, t = 0.0, xo = 0.0, yo = 0.0;

    for(unsigned int i = 0; i < c; i+=2) {
        if (orientation == TopLeftToBottomRight) {
            l = (i / 2 % xCount) * w;
            t = (c / 2.0 - 2 - i / 2 / xCount) * h;
        } else {
            l = (c / 2.0 - 2 - i / 2 % xCount) * w;
            t = (i / 2 / xCount) * h;
        }

        if (plotBoxes[i] != 0) {
            plotBoxes[i]->reset(l, t, w, h);

            xo = plotBoxes[i]->width * Legend::XOffset;
            yo = plotBoxes[i]->height * Legend::YOffset;
        }

        if (plotBoxes[i+1] != 0) {
            plotBoxes[i+1]->reset(l+xo, t+yo, w-xo, h-yo);
        }
    }
}

//! @todo this should also call a repaint in window
void GridLayout::remap(unsigned int xCount, unsigned int yCount)
{
    this->xCount = xCount;
    this->yCount = yCount;
    recalculate();
}
}
