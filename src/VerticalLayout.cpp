#include "VerticalLayout.hpp"

namespace hpl
{
VerticalLayout::VerticalLayout()
{
}

VerticalLayout::~VerticalLayout()
{
}

//! @todo this assumes we always have legend, then plot. does not always have to be like this -> find more general form
void VerticalLayout::recalculate()
{
    unsigned int c = plotBoxes.size();
    double w = 1.0;
    double h = 1.0 / (c / 2.0);

    double l = 0, t = 0;
    for(unsigned int i = 0; i < c; i++) {
        plotBoxes[i]->reset(l, t, w, h);

        float xo = plotBoxes[i]->width * Legend::XOffset;
        float yo = plotBoxes[i]->height * Legend::YOffset;
        i++;
        if (i < c) {
            plotBoxes[i]->reset(l+xo, t+yo, w-xo, h-yo);
        }

        t += h;
    }
}
}

