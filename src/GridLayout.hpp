#ifndef GRIDLAYOUT_HPP
#define GRIDLAYOUT_HPP

#include "Layout.hpp"

namespace hpl
{
class GridLayout : public Layout
{

public:
    GridLayout(unsigned int xCount, unsigned int yCount);
    virtual ~GridLayout();

    void remap(unsigned int xCount, unsigned int yCount);

    virtual void recalculate();

private:
    unsigned int xCount, yCount;

};
}

#endif // GRIDLAYOUT_HPP
