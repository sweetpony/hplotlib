#ifndef PLOTPRINTER_HPP
#define PLOTPRINTER_HPP

#include <string>
#include <map>
#include <vector>

#include "Plot.hpp"
#include "Lines.hpp"
#include "Points.hpp"
#include "Legend.hpp"

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

    virtual bool saveToFile(const std::string& fileName, std::vector<Plot*> plots) = 0;

protected:
    typedef std::pair<unsigned int, unsigned int> Pixel;
    Pixel transformCoordinates(double x, double y) const;

    bool adjustMinMax(PlotPart* pp, bool alwaysUse);

    double minX = 0, minY = 0, maxX = 1.0, maxY = 1.0;
    unsigned int ppi = 72, dpi = 150;
    unsigned int pixelX = 595 * ppi * 1.0 / dpi, pixelY = 842 * ppi * 1.0 / dpi;
    Orientation orientation = Landscape;

};
}

#endif // PLOTPRINTER_HPP
