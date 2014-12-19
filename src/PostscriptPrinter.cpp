#include "PostscriptPrinter.hpp"

namespace hpl
{
PostscriptPrinter::PostscriptPrinter(Orientation orientation) : AbstractPlotter(), PlotPrinter(orientation)
{
}

PostscriptPrinter::~PostscriptPrinter()
{
    out.close();
}

bool PostscriptPrinter::saveToFile(const std::string& fileName)
{
    this->fileName = fileName + ".ps";
    out.open(this->fileName);
    if (! out.is_open()) {
        return false;
    }
    update();
    return true;
}

void PostscriptPrinter::update()
{
    writeHeader();
    setFont(10);

    for(auto i = plots->cbegin(); i != plots->cend(); i++) {
        setCurrentGeometry(i->second->getGeometry(), i->second->xmin(), i->second->xmax(), i->second->ymin(), i->second->ymax());
        Lines* l = dynamic_cast<Lines*>(i->second);
        if (l != 0) {
            setColor(l->getColor());
            setLineWidth(l->getThickness());
            draw(l->n(), l->x(), l->y(), l->getDataType());
            continue;
        }
        Points* p = dynamic_cast<Points*>(i->second);
        if (p != 0) {
            setColor(p->getColor());
            if (p->getSymbol() != Points::Dot) {
                setLineWidth(p->getSymbolSize());
            }
            draw(p->n(), p->x(), p->y(), p->getDataType());
            continue;
        }
        Contour* c = dynamic_cast<Contour*>(i->second);
        if (c != 0 && c->getDataType() == Drawable::Type_Texture) {
            setCurrentZLimits(c->getZmin(), c->getZmax());
            draw(c->n, c->x, c->y, c->z, c->getColorTable());
        }
    }
    writeFooter();
}

void PostscriptPrinter::writeHeader()
{
    out << "%!PS-Adobe-3.0" << std::endl;
    out << "%%BoundingBox 0 0 " << (pixelX+2*pixelBoundary)*sizefactor << " " << (pixelY+2*pixelBoundary)*sizefactor << std::endl;
}

void PostscriptPrinter::writeFooter()
{
    out << "%%EOF" << std::endl;
}

void PostscriptPrinter::setFont(unsigned int size)
{
    out << "/Inconsolata findfont" << std::endl;
    out << size << " scalefont" << std::endl;
    out << "setfont" << std::endl;
}

void PostscriptPrinter::setColor(const Color& color)
{
    out << color.r << " " << color.g << " " << color.b << " setrgbcolor" << std::endl;
}

void PostscriptPrinter::setLineWidth(unsigned int width)
{
    out << width << " setlinewidth" << std::endl;
}

void PostscriptPrinter::draw(int n, double const* x, double const* y, Drawable::Type type)
{
    switch (type) {
        case Drawable::Type_Lines:
            for (int i = 0; i < n-1; i+=2) {
                //! @todo this should actually be done in the Drawable derivates
                if (isfinite(x[i], y[i]) && isfinite(x[i+1], y[i+1])) {
                    drawLine(x[i], y[i], x[i+1], y[i+1]);
                }
            }
            break;
        case Drawable::Type_LineStrips:
            for (int i = 0; i < n-1; ++i) {
                if (isfinite(x[i], y[i]) && isfinite(x[i+1], y[i+1])) {
                    drawLine(x[i], y[i], x[i+1], y[i+1]);
                }
            }
            break;
        case Drawable::Type_Points:
            for (int i = 0; i < n; ++i) {
                if (isfinite(x[i], y[i])) {
                    drawPoint(x[i], y[i]);
                }
            }
            break;
        case Drawable::Type_Texture:
            break;
    }
}

void PostscriptPrinter::draw(int n, double const* x, double const* y, double const* z, ColorTable const& ct)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double zi = (z[i*n+j] - currentZMin) / (currentZMax - currentZMin);
            unsigned int imin = static_cast<unsigned int>(zi * ct.num);
            unsigned int imax = static_cast<unsigned int>(ceil(zi * ct.num));
            if (imax >= ct.num) {
                imax = ct.num - 1;
            }
            double delta = zi * ct.num - imin;
            float r = ct.r[imin] * delta + ct.r[imax] * (1.0 - delta);
            float g = ct.g[imin] * delta + ct.g[imax] * (1.0 - delta);
            float b = ct.b[imin] * delta + ct.b[imax] * (1.0 - delta);
            setColor(Color(r, g, b));

            double dxp = 0.5 * (x[i] - (i == 0 ? currentXMin : x[i-1]));
            double dyp = 0.5 * (y[i] - (i == 0 ? currentYMin : y[i-1]));
            double dxn = 0.5 * ((i == 0 ? currentXMax : x[i+1]) - x[i]);
            double dyn = 0.5 * ((i == 0 ? currentYMax : y[i+1]) - y[i]);

            fillShape({x[i]-dxp, x[i]-dxp, x[i]+dxn, x[i]+dxn}, {y[i]-dyp, y[i]+dyn, y[i]+dyn, y[i]-dyp});
        }
    }
}

void PostscriptPrinter::drawLine(double x1, double y1, double x2, double y2)
{
    Pixel p1 = transformCoordinates(x1, y1), p2 = transformCoordinates(x2, y2);
    out << "newpath" << std::endl;
    out << p1.first << " " << p1.second << " moveto" << std::endl;
    out << p2.first << " " << p2.second << " lineto" << std::endl;
    out << "stroke" << std::endl;
}

void PostscriptPrinter::drawPoint(double x, double y)
{
    Pixel p = transformCoordinates(x, y);
    out << "newpath" << std::endl;
    out << p.first << " " << p.second << " moveto" << std::endl;
    out << "gsave currentpoint lineto 1 setlinecap stroke grestore" << std::endl;
}

void PostscriptPrinter::fillShape(std::vector<double> x, std::vector<double> y)
{
    if (x.size() == 0 || y.size() == 0) {
        return;
    }

    out << "newpath" << std::endl;
    Pixel p = transformCoordinates(x[0], y[0]);
    out << p.first << " " << p.second << " moveto" << std::endl;
    for (unsigned int i = 1; i < x.size() && i < y.size(); i++) {
        p = transformCoordinates(x[i], y[i]);
        out << p.first << " " << p.second << " lineto" << std::endl;
    }
    out << "closepath" << std::endl;
    out << "gsave fill grestore" << std::endl;
}

void PostscriptPrinter::writeText(double x, double y, std::string text)
{
    Pixel p = transformCoordinates(x, y);
    out << p.first << " " << p.second << " moveto" << std::endl;
    out << "(" << text << ") show" << std::endl;
}
}
