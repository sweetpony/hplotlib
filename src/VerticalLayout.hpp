#ifndef VERTICALLAYOUT_HPP
#define VERTICALLAYOUT_HPP

#include "Layout.hpp"

namespace hpl
{
class VerticalLayout : public Layout
{

public:
    enum Orientation {
        TopToBottom,
        BottomToTop
    };

    VerticalLayout(Orientation orientation = TopToBottom);
    virtual ~VerticalLayout();

    virtual void recalculate();

    void changeOrientation(Orientation orientation);

private:
    Orientation orientation;

};
}

#endif // VERTICALLAYOUT_HPP
