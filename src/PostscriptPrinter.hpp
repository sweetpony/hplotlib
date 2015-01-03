#ifndef POSTSCRIPTPRINTER_HPP
#define POSTSCRIPTPRINTER_HPP

#include <ostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "AbstractPlotter.hpp"
#include "PlotPrinter.hpp"
#include "Color.hpp"

namespace hpl
{
class PostscriptPrinter : public AbstractPlotter, public PlotPrinter
{

public:
    PostscriptPrinter(Orientation orientation = Landscape);
    virtual ~PostscriptPrinter();

    virtual bool saveToFile(const std::string& fileName);
    virtual void update();

private:
    void writeHeader();
    void writeFooter();
    void setFont(std::string fontname, unsigned int size);
    void setColor(const Color& color);
    void setLineWidth(unsigned int width);

    void draw(int n, double const* x, double const* y, Drawable::Type type);
    void draw(int n, double const* x, double const* y, Color const* colors);

    void drawLine(double x1, double y1, double x2, double y2);
    void drawPoint(double x, double y);
    void fillShape(std::vector<double> x, std::vector<double> y);
    void writeText(double x, double y, std::string text);

    inline bool isfinite(double x, double y) const {
        return std::isfinite(x) && std::isfinite(y);
    }

    std::ofstream out;
    std::string fileName;

};
}

#endif // POSTSCRIPTPRINTER_HPP
