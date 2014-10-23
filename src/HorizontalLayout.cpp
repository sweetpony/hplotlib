#include "HorizontalLayout.hpp"

namespace hpl
{
HorizontalLayout::HorizontalLayout()
{
}

HorizontalLayout::~HorizontalLayout()
{
}

void HorizontalLayout::recalculate()
{
    unsigned int c = plotBoxes.size();
    double w = 1.0 / c;
    double h = 1.0;

    double l = 0, t = 0;
    for(unsigned int i = 0; i < c; i++) {
        plotBoxes[i]->reset(l, t, w, h);
        l += w;
    }
}
}
