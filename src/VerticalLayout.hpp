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

    VerticalLayout() : Layout() {}
    virtual ~VerticalLayout() {}

    void changeOrientation(Orientation orientation);

private:
    virtual void calculate(std::vector<Geometry>& geometries);
    Orientation orientation = TopToBottom;
};
}

#endif // VERTICALLAYOUT_HPP
