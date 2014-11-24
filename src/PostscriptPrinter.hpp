#ifndef POSTSCRIPTPRINTER_HPP
#define POSTSCRIPTPRINTER_HPP

#include <ostream>
#include <fstream>
#include <vector>

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
    void setFont(unsigned int size);
    void setColor(const Color& color);
    void setLineWidth(unsigned int width);

    void draw(int n, double const* x, double const* y, Drawable::Type type, const Geometry& geometry);

    void drawLine(double x1, double y1, double x2, double y2, const Geometry& geometry);
    void drawPoint(double x, double y, const Geometry& geometry);
    void fillShape(std::vector<double> x, std::vector<double> y, const Geometry& geometry);
    void writeText(double x, double y, std::string text, const Geometry& geometry);

    std::ofstream out;
    std::string fileName;

};
}

#endif // POSTSCRIPTPRINTER_HPP
