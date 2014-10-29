#include "PostscriptPrinter.hpp"

namespace hpl
{
PostscriptPrinter::PostscriptPrinter(Orientation orientation) : PlotPrinter(orientation)
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

    writeHeader(o);
    for(std::vector<Plot*>::iterator i = plots.begin(); i != plots.end(); i++) {
        //! @todo also handle legend in the following two statements
        std::vector<PlotPart*> parts = (*i)->getPlotParts();
        for (std::vector<PlotPart*>::iterator j = parts.begin(); j != parts.end(); j++) {
            Lines* l = dynamic_cast<Lines*>(*j);
            if (l != 0) {
                float* x = l->getX();
                float* y = l->getY();

                for (int k = 0; k < l->getN()-1; k++) {
                    drawLine(o, x[k], y[k], x[k+1], y[k+1]);
                }

                delete[] x;
                delete[] y;
                continue;
            }
            Points* p = dynamic_cast<Points*>(*j);
            if (p != 0) {
                float* x = p->getX();
                float* y = p->getY();

                //! @todo set point size
                for (int k = 0; k < p->getN(); k++) {
                    drawPoint(o, x[k], y[k], 1);
                }

                delete[] x;
                delete[] y;
                continue;
            }
        }
        Legend* l = (*i)->getLegend();
        if (l != 0) {
            //! @todo draw legend
            float* lines = l->getLines();

            for (int k = 0; k < l->getLinesCount()-3; k+=4) {
                drawLine(o, lines[k], lines[k+1], lines[k+2], lines[k+3]);
            }

            delete[] lines;
        }
    }
    writeFooter(o);

    o.close();
    return true;
}

void PostscriptPrinter::writeHeader(std::ofstream& o) const
{
    o << "%!PS-Adobe-3.0" << std::endl;
    o << "%%BoundingBox 0 0 " << pixelX << " " << pixelY << std::endl;
}

void PostscriptPrinter::writeFooter(std::ofstream& o) const
{
    o << "%%EOF" << std::endl;
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

void PostscriptPrinter::drawPoint(std::ofstream& o, double x, double y, unsigned int size) const
{
    Pixel p = transformCoordinates(x, y);
    o << "newpath" << std::endl;
    o << p.first << " " << p.second << " moveto" << std::endl;
    o << "gsave currentpoint lineto " << size << " setlinewidth 1 setlinecap stroke grestore" << std::endl;
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
