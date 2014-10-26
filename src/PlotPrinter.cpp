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
    p.first = x * pixelX;
    p.second = y * pixelY;
    return p;
}
}
