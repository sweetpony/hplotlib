#ifndef GRIDLAYOUT_HPP
#define GRIDLAYOUT_HPP

#include "Layout.hpp"

#include <limits>

namespace hpl
{
class GridLayout : public Layout
{

public:
    enum Orientation {
        TopLeftToBottomRight,
        TopRightToBottomLeft,
        BottomRightToTopLeft,
        BottomLeftToTopRight,
    };

	GridLayout() : Layout() {}
    virtual ~GridLayout() {}

    void setMaxRows(unsigned mr);
    void setMaxCols(unsigned mc);
    void changeOrientation(Orientation orientation);

protected:
    virtual void calculate(std::vector<Geometry>& geometries);

private:
    unsigned maxrows = std::numeric_limits<unsigned>::max();
    unsigned maxcols = std::numeric_limits<unsigned>::max();
    Orientation orientation = TopLeftToBottomRight;

};
}

#endif // GRIDLAYOUT_HPP
