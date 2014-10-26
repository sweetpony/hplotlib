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

bool PlotPrinter::adjustMinMax(PlotPart* pp, bool alwaysUse)
{
    Lines* l = dynamic_cast<Lines*>(pp);
    if (l != 0) {
        if (alwaysUse || l->getXMin() < minX) {
            minX = l->getXMin();
        }
        if (alwaysUse || l->getYMin() < minY) {
            minY = l->getYMin();
        }
        if (alwaysUse || l->getXMax() < maxX) {
            maxX = l->getXMax();
        }
        if (alwaysUse || l->getYMax() < maxY) {
            maxY = l->getYMax();
        }
        return true;
    }
    Points* p = dynamic_cast<Points*>(pp);
    if (p != 0) {
        if (alwaysUse || p->getXMin() < minX) {
            minX = p->getXMin();
        }
        if (alwaysUse || p->getYMin() < minY) {
            minY = p->getYMin();
        }
        if (alwaysUse || p->getXMax() < maxX) {
            maxX = p->getXMax();
        }
        if (alwaysUse || p->getYMax() < maxY) {
            maxY = p->getYMax();
        }
        return true;
    }
    Legend* le = dynamic_cast<Legend*>(pp);
    if (le != 0) {
        //! @todo retreive coords and return true
        return false;
    }
    return false;
}
}
