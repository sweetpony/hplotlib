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

void PlotPrinter::setCurrentGeometry(const Geometry& geometry, double xmin, double xmax, double ymin, double ymax)
{
    currentGeometry = &geometry;
    currentXMin = xmin;
    currentXMax = xmax;
    currentYMin = ymin;
    currentYMax = ymax;
}

PlotPrinter::Pixel PlotPrinter::transformCoordinates(double x, double y) const
{
    Pixel p;
    // data -> interleave -> in geometry -> in output window
    p.first = ((x - currentXMin) / (currentXMax - currentXMin) * currentGeometry->width + currentGeometry->leftOffset) * pixelX;
    p.second = ((y - currentYMin) / (currentYMax - currentYMin) * currentGeometry->height + currentGeometry->topOffset) * pixelY;
    return p;
}
}
