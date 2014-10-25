#ifndef HORIZONTALLAYOUT_HPP
#define HORIZONTALLAYOUT_HPP

#include "Layout.hpp"

namespace hpl
{
class HorizontalLayout : public Layout
{

public:
    enum Orientation {
        LeftToRight,
        RightToLeft
    };

    HorizontalLayout(Orientation orientation = LeftToRight);
    virtual ~HorizontalLayout();

    virtual void recalculate();

    void changeOrientation(Orientation orientation);

private:
    Orientation orientation;

};
}

#endif // HORIZONTALLAYOUT_HPP
