#include "PlotPrinter.hpp"

namespace hpl
{
PlotPrinter::PlotPrinter(Orientation orientation) : orientation(orientation)
{
    if (orientation == Portrait) {
        unsigned int buf = pixelX;
        pixelX = pixelY;
        pixelY = buf;
    }
}

PlotPrinter::~PlotPrinter()
{
}

PlotPrinter::Pixel PlotPrinter::transformCoordinates(double x, double y, const Geometry& geometry) const
{
    Pixel p;
    p.first = (x * geometry.width + geometry.leftOffset) * pixelX;
    p.second = (y* geometry.height + geometry.topOffset) * pixelY;
    return p;
}
}
