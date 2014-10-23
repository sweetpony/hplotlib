#ifndef FIXEDLAYOUT_HPP
#define FIXEDLAYOUT_HPP

#include "Layout.hpp"

namespace hpl
{
class FixedLayout : public Layout
{

public:
    FixedLayout();
    virtual ~FixedLayout();

    virtual void recalculate();

};
}

#endif // FIXEDLAYOUT_HPP
