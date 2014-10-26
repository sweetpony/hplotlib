#include "PostscriptPrinter.hpp"

namespace hpl
{
PostscriptPrinter::PostscriptPrinter()
{
}

PostscriptPrinter::~PostscriptPrinter()
{
}

bool PostscriptPrinter::saveToFile(const std::string& fileName, std::vector<Plot*> plots)
{
    std::ofstream o(fileName + ".ps");
    if (! o.is_open()) {
        return false;
    }

    //! @todo Set min, max and pixel attributes appropriately
    minX = minY = 0.0;
    maxX = maxY = 1.0;
    pixelX = pixelY = 1000;
    //! @todo Write plot data to file, this is just a test
    writeHeader(o);
    drawLine(o, 0.1, 0.1, 0.9, 0.4);
    setFont(o, 15);
    setColor(o, Color(1.0f, 0.0f, 0.0f));
    writeText(o, 0.2, 0.5, "Hello Postscript");

    o.close();
    return true;
}

void PostscriptPrinter::writeHeader(std::ofstream& o) const
{
    o << "%%BoundingBox 0 0 " << pixelX << " " << pixelY << std::endl;
}

void PostscriptPrinter::setFont(std::ofstream& o, unsigned int size) const
{
    o << "/Inconsolata findfont" << std::endl;
    o << size << " scalefont" << std::endl;
    o << "setfont" << std::endl;
}

void PostscriptPrinter::setColor(std::ofstream& o, const Color& color) const
{
    o << color.r << " " << color.g << " " << color.b << " setrgbcolor" << std::endl;
}

void PostscriptPrinter::setLineWidth(std::ofstream& o, unsigned int width) const
{
    o << width << " setlinewidth" << std::endl;
}

void PostscriptPrinter::drawLine(std::ofstream& o, double x1, double y1, double x2, double y2) const
{
    Pixel p1 = transformCoordinates(x1, y1), p2 = transformCoordinates(x2, y2);
    o << "newpath" << std::endl;
    o << p1.first << " " << p1.second << " moveto" << std::endl;
    o << p2.first << " " << p2.second << " lineto" << std::endl;
    o << "stroke" << std::endl;
}

void PostscriptPrinter::fillShape(std::ofstream& o, std::vector<double> x, std::vector<double> y) const
{
    if (x.size() == 0 || y.size() == 0) {
        return;
    }

    o << "newpath" << std::endl;
    Pixel p = transformCoordinates(x[0], y[0]);
    o << p.first << " " << p.second << " moveto" << std::endl;
    for (unsigned int i = 1; i < x.size() && i < y.size(); i++) {
        p = transformCoordinates(x[i], y[i]);
        o << p.first << " " << p.second << " lineto" << std::endl;
    }
    o << "closepath" << std::endl;
    o << "gsave fill grestore" << std::endl;
}

void PostscriptPrinter::writeText(std::ofstream& o, double x, double y, std::string text) const
{
    Pixel p = transformCoordinates(x, y);
    o << p.first << " " << p.second << " moveto" << std::endl;
    o << "(" << text << ") show" << std::endl;
}
}
