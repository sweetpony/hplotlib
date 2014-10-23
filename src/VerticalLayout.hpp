#ifndef VERTICALLAYOUT_HPP
#define VERTICALLAYOUT_HPP

#include "Layout.hpp"

namespace hpl
{
class VerticalLayout : public Layout
{

public:
    VerticalLayout();
    virtual ~VerticalLayout();

    virtual void recalculate();

};
}

#endif // VERTICALLAYOUT_HPP
