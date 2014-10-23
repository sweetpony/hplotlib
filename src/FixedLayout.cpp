#include "FixedLayout.hpp"

namespace hpl
{
FixedLayout::FixedLayout()
{
}

FixedLayout::~FixedLayout()
{
}

void FixedLayout::recalculate()
{
    for(unsigned int i = 0; i < plotBoxes.size(); i+=2) {
        if (plotBoxes[i] != 0 && plotBoxes[i+1] != 0) {
            double xo = plotBoxes[i]->width * Legend::XOffset;
            double yo = plotBoxes[i]->height * Legend::YOffset;

            plotBoxes[i+1]->reset(plotBoxes[i]->leftOffset+xo, plotBoxes[i]->topOffset+yo, plotBoxes[i]->width-xo, plotBoxes[i]->height-yo);
        }
    }
}
}
