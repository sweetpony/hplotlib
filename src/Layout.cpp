#include "Layout.hpp"

namespace hpl
{
void Layout::get(std::vector<Geometry>& geometries)
{
	calculate(geometries);	
    for(auto it = geometries.begin(); it != geometries.end(); ++it) {
		it->leftOffset += geometry.leftOffset;
		it->topOffset += geometry.topOffset;
		it->width *= geometry.width;
		it->height *= geometry.height;
	}
}
}
