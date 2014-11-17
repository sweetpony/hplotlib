#include "Lines.hpp"

namespace hpl {
void Lines::setThickness(double thick)
{
    //! @todo recalculate lines
    thickness = thick;
    changed.invoke(plotId);
}

void Lines::setStyle(Style s)
{
    //! @todo recalculate lines, perhaps just toggle _seperate, could do the job for dashed!
    style = s;
    changed.invoke(plotId);
}
}
