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
    Pixel transformCoordinates(double x, double y) const;

    unsigned int pixelX = 612, pixelY = 792;
    Orientation orientation = Landscape;

private:
    Geometry currentGeometry;
    double currentXMin = 0.0, currentXMax = 1.0, currentYMin = 0.0, currentYMax = 1.0;

};
}

#endif // PLOTPRINTER_HPP
