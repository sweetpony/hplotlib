#include "HorizontalLayout.hpp"

namespace hpl
{
HorizontalLayout::HorizontalLayout(Orientation orientation) : Layout(), orientation(orientation)
{
}

HorizontalLayout::~HorizontalLayout()
{
}

void HorizontalLayout::recalculate(std::vector<Geometry>& geometries)
{
    unsigned int c = geometries.size();
    double w = 1.0 / c;
    double h = 1.0;
    double t = 0.0;

	double l = 0.0;
    for(auto it = geometries.begin(); it != geometries.end(); ++it) {
		if (orientation == RightToLeft) {
			it->set(1.0 - l - w, t, w, h);
		} else {
			it->set(l, t, w, h);
		}
		l += w;
    }
}

void HorizontalLayout::changeOrientation(Orientation orientation)
{
    //~ this->orientation = orientation;
    //~ recalculate();
    //~ changed.invoke();
}
}
