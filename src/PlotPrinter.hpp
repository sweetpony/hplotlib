#ifndef PLOTPRINTER_HPP
#define PLOTPRINTER_HPP

#include <string>
#include <map>

#include "Plot.hpp"
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

    virtual bool saveToFile(const std::string& fileName, const Registry<CoordinateSystem>& plots) = 0;

protected:
    typedef std::pair<unsigned int, unsigned int> Pixel;
    Pixel transformCoordinates(double x, double y) const;

    unsigned int pixelX = 612, pixelY = 792;
    Orientation orientation = Landscape;

};
}

#endif // PLOTPRINTER_HPP
