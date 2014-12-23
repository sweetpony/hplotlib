#include "PlotPrinter.hpp"

namespace hpl
{
PlotPrinter::PlotPrinter(Orientation orientation) : orientation(orientation)
{
    if (orientation == Landscape) {
        unsigned int buf = pixelX;
        pixelX = pixelY;
        pixelY = buf;
    }
}

PlotPrinter::~PlotPrinter()
{
}

void PlotPrinter::setOrientation(Orientation orientation)
{
    if (this->orientation != orientation) {
        unsigned int buf = pixelX;
        pixelX = pixelY;
        pixelY = buf;
    }
    this->orientation = orientation;
    update();
}

void PlotPrinter::setPageSize(PageSize page)
{
    switch (page) {
    case Letter:
        setPageSize(612, 792);
        break;
    case A4:
        setPageSize(595, 842);
        break;
    case A5:
        setPageSize(420, 595);
        break;
    }
}

void PlotPrinter::setPageSize(double x, double y)
{
    if (orientation == Portrait) {
        pixelX = x;
        pixelY = y;
    } else {
        pixelX = y;
        pixelY = x;
    }
}

void PlotPrinter::setCurrentGeometry(const Geometry& geometry, double xmin, double xmax, double ymin, double ymax)
{
    currentGeometry = geometry;
    currentXMin = xmin;
    currentXMax = xmax;
    currentYMin = ymin;
    currentYMax = ymax;
}

void PlotPrinter::setCurrentZLimits(double zmin, double zmax)
{
    currentZMin = zmin;
    currentZMax = zmax;
}

PlotPrinter::Pixel PlotPrinter::transformCoordinates(double x, double y) const
{
    Pixel p;
    // data -> interleave -> in geometry -> in output window
    p.first = (((x - currentXMin) / (currentXMax - currentXMin) * currentGeometry.width + currentGeometry.leftOffset) * pixelX + pixelBoundary) * sizefactor;
    p.second = (((y - currentYMin) / (currentYMax - currentYMin) * currentGeometry.height + currentGeometry.topOffset) * pixelY + pixelBoundary) * sizefactor;
    return p;
}
}
