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

PlotPrinter::Pixel PlotPrinter::transformCoordinates(double x, double y) const
{
    Pixel p;
    p.first = (x - minX) * pixelX / (maxX - minX);
    p.second = (y - minY) * pixelY / (maxY - minY);
    return p;
}
}
