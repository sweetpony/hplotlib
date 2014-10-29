#include "HorizontalLayout.hpp"

namespace hpl
{
void HorizontalLayout::calculate(std::vector<Geometry>& geometries)
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
    this->orientation = orientation;
    changed.invoke(id());
}
}
