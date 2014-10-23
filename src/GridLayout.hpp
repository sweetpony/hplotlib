#ifndef GRIDLAYOUT_HPP
#define GRIDLAYOUT_HPP

#include "Layout.hpp"

namespace hpl
{
class GridLayout : public Layout
{

public:
    GridLayout();
    virtual ~GridLayout();

    virtual void recalculate();

};
}

#endif // GRIDLAYOUT_HPP
