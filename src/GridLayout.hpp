#ifndef GRIDLAYOUT_HPP
#define GRIDLAYOUT_HPP

#include "Layout.hpp"

namespace hpl
{
class GridLayout : public Layout
{

public:
    enum Orientation {
        TopLeftToBottomRight,
        BottomRightToTopLeft
    };

    GridLayout(unsigned int xCount, unsigned int yCount, Orientation orientation = TopLeftToBottomRight);
    virtual ~GridLayout();

    void remap(unsigned int xCount, unsigned int yCount);
    void changeOrientation(Orientation orientation);

    virtual void recalculate();

private:
    unsigned int xCount, yCount;
    Orientation orientation;

};
}

#endif // GRIDLAYOUT_HPP
