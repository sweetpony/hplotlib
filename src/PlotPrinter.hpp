#ifndef PLOTPRINTER_HPP
#define PLOTPRINTER_HPP

#include <string>
#include <map>

#include "Drawable.hpp"
#include "Lines.hpp"
#include "Points.hpp"
#include "CoordinateSystem.hpp"
#include "Registry.hpp"

namespace hpl
{
class PlotPrinter
{

public:
    enum Orientation {
        Landscape,
        Portrait
    };

    PlotPrinter(Orientation orientation = Landscape);
    virtual ~PlotPrinter();

    virtual bool saveToFile(const std::string& fileName) = 0;

protected:
    typedef std::pair<unsigned int, unsigned int> Pixel;

    void setCurrentGeometry(const Geometry& geometry, double xmin, double xmax, double ymin, double ymax);
    void setCurrentZLimits(double zmin, double zmax);
    Pixel transformCoordinates(double x, double y) const;

    unsigned int pixelX = 612, pixelY = 792, pixelBoundary = 20;
    double sizefactor = 4.0;
    Orientation orientation = Landscape;

    double currentXMin = 0.0, currentXMax = 1.0, currentYMin = 0.0, currentYMax = 1.0, currentZMin = 0.0, currentZMax = 1.0;
private:
    Geometry currentGeometry;

};
}

#endif // PLOTPRINTER_HPP
