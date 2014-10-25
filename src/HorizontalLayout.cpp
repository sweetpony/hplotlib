#include "HorizontalLayout.hpp"

namespace hpl
{
HorizontalLayout::HorizontalLayout(Orientation orientation) : Layout(), orientation(orientation)
{
}

HorizontalLayout::~HorizontalLayout()
{
}

void HorizontalLayout::recalculate()
{
    unsigned int c = plotBoxes.size();
    double w = 1.0 / (c / 2.0);
    double h = 1.0;

    double l = 0.0, t = 0.0, xo = 0.0, yo = 0.0;
    if (orientation == RightToLeft) {
        l = (c / 2.0 - 1) * w;
    }

    for(unsigned int i = 0; i < c; i+=2) {
        if (plotBoxes[i] != 0) {
            plotBoxes[i]->reset(l, t, w, h);

            xo = plotBoxes[i]->width * Legend::XOffset;
            yo = plotBoxes[i]->height * Legend::YOffset;
        }

        if (plotBoxes[i+1] != 0) {
            plotBoxes[i+1]->reset(l+xo, t+yo, w-xo, h-yo);
        }

        if (orientation == LeftToRight) {
            l += w;
        } else {
            l -= w;
        }
    }
}
}
