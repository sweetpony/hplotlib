#include "VerticalLayout.hpp"

namespace hpl
{
VerticalLayout::VerticalLayout()
{
}

VerticalLayout::~VerticalLayout()
{
}

void VerticalLayout::recalculate()
{
    unsigned int c = plotBoxes.size();
    double w = 1.0;
    double h = 1.0 / c;

    double l = 0, t = 0;
    for(unsigned int i = 0; i < c; i++) {
        plotBoxes[i]->reset(l, t, w, h);
        t += h;
    }
}
}

