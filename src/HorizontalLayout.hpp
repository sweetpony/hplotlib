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

    HorizontalLayout() : Layout() {}
    virtual ~HorizontalLayout() {}

    void changeOrientation(Orientation orientation);

protected:
    virtual void calculate(std::vector<Geometry>& geometries);

private:
    Orientation orientation = LeftToRight;

};
}

#endif // HORIZONTALLAYOUT_HPP
