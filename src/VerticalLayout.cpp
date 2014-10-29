#include "VerticalLayout.hpp"

namespace hpl
{
void VerticalLayout::calculate(std::vector<Geometry>& geometries)
{
	unsigned int c = geometries.size();
	double w = 1.0;
    double h = 1.0 / c;
    double t = 0.0;
	double l = 0.0;
    for(auto it = geometries.begin(); it != geometries.end(); ++it) {
		if (orientation == TopToBottom) {
			it->set(l, 1.0 - t - h, w, h);
		} else {
			it->set(l, t, w, h);
		}
		t += h;
    }
}

void VerticalLayout::changeOrientation(Orientation orientation)
{
    this->orientation = orientation;
    changed.invoke(id());
}
}

