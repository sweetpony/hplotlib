#include "GridLayout.hpp"

#include <cmath>

namespace hpl
{
void GridLayout::calculate(std::vector<Geometry>& geometries)
{
	unsigned rows = round(sqrt(geometries.size()));
	if (rows > maxrows) {
		rows = maxrows;
	}
	unsigned cols = ceil(static_cast<double>(geometries.size()) / rows);
	if (cols > maxcols) {
		cols = maxcols;
		rows = ceil(static_cast<double>(geometries.size()) / cols);
	}
	double w = 1.0 / cols;
	double h = 1.0 / rows;
	unsigned j = 0;
    for(auto it = geometries.begin(); it != geometries.end(); ++it) {
		double x;
		double y;
		if (orientation == TopLeftToBottomRight || orientation == TopRightToBottomLeft) {
			y = 1.0 - h - floor(j / cols) * h;
		} else {
			y = floor(j / cols) * h;
		}
		if (orientation == TopLeftToBottomRight || orientation == BottomLeftToTopRight) {
			x = (j % cols) * w;
		} else {
			x = 1.0 - w - (j % cols) * w;
		}
		it->set(x, y, w, h);
		++j;
    }
}

void GridLayout::setMaxRows(unsigned mr)
{
	maxcols = std::numeric_limits<unsigned>::max();
	maxrows = mr;
	changed.invoke(id());
}

void GridLayout::setMaxCols(unsigned mc)
{
	maxrows = std::numeric_limits<unsigned>::max();
	maxcols = mc;
	changed.invoke(id());
}

void GridLayout::changeOrientation(Orientation orientation)
{
    this->orientation = orientation;
    changed.invoke(id());
}
}
