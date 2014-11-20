#include "Lines.hpp"

namespace hpl {
void Lines::setThickness(double thick)
{
    thickness = thick;
    changed.invoke(plotId);
}

// Assume _seperate is always false for relevant cases. Can do this as long as _seperate is a hidden property only to distinguish coordinate system and real plots
void Lines::setStyle(Style s)
{
    if (s != style) {
        delete lines;

        switch(s) {
            case Solid:
                lines = new SimpleLines(_n, _x, _y, _separate, false);
                break;
            case Dashed:
                //! @todo perhaps want to improve this such that beginning and end are connected by visible lines no matter if n % 2 is 0 or 1
                lines = new SimpleLines(_n, _x, _y, !_separate, false);
                break;
            case Dotted:
                //! @todo implement, have to interpolate the curve
                break;
        }

        style = s;
        type = (lines->_separate ? Type_Lines : Type_LineStrips);
        changed.invoke(plotId);
    }
}
}
