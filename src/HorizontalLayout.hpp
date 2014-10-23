#ifndef HORIZONTALLAYOUT_HPP
#define HORIZONTALLAYOUT_HPP

#include "Layout.hpp"

namespace hpl
{
class HorizontalLayout : public Layout
{

public:
    HorizontalLayout();
    virtual ~HorizontalLayout();

    virtual void recalculate();

};
}

#endif // HORIZONTALLAYOUT_HPP
