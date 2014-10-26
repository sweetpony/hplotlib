#ifndef PLOTPRINTER_HPP
#define PLOTPRINTER_HPP

#include <string>
#include <map>
#include <vector>

#include "Plot.hpp"

namespace hpl
{
class PlotPrinter
{

public:
    PlotPrinter();
    virtual ~PlotPrinter();

    virtual bool saveToFile(const std::string& fileName, std::vector<Plot*> plots) = 0;

protected:
    typedef std::pair<unsigned int, unsigned int> Pixel;
    Pixel transformCoordinates(double x, double y) const;

    double minX, minY, maxX, maxY;
    unsigned int pixelX, pixelY;

};
}

#endif // PLOTPRINTER_HPP
