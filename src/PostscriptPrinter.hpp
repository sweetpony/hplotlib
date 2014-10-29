#ifndef POSTSCRIPTPRINTER_HPP
#define POSTSCRIPTPRINTER_HPP

#include <ostream>
#include <fstream>
#include <vector>

#include "PlotPrinter.hpp"
#include "Color.hpp"

namespace hpl
{
class PostscriptPrinter : public PlotPrinter
{

public:
    PostscriptPrinter(Orientation orientation = Landscape);
    virtual ~PostscriptPrinter();

    virtual bool saveToFile(const std::string& fileName, std::vector<Plot*> plots);

private:
    void writeHeader(std::ofstream& o) const;
    void writeFooter(std::ofstream& o) const;
    void setFont(std::ofstream& o, unsigned int size) const;
    void setColor(std::ofstream& o, const Color& color) const;
    void setLineWidth(std::ofstream& o, unsigned int width) const;
    void drawLine(std::ofstream& o, double x1, double y1, double x2, double y2) const;
    void drawPoint(std::ofstream& o, double x, double y, unsigned int size) const;
    void fillShape(std::ofstream& o, std::vector<double> x, std::vector<double> y) const;
    void writeText(std::ofstream& o, double x, double y, std::string text) const;

};
}

#endif // POSTSCRIPTPRINTER_HPP
