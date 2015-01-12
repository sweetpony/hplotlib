#include "PostscriptPrinter.hpp"

namespace hpl
{
PostscriptPrinter::PostscriptPrinter(Orientation orientation) : AbstractPlotter(), PlotPrinter(orientation)
{
}

PostscriptPrinter::~PostscriptPrinter()
{
    out.close();
    delete lastColor;
    delete lastFont;
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
    if (plots == nullptr) {
        return;
    }

    writeHeader();

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
            Color* colors = c->getColors();
            draw(c->n, c->x, c->y, colors);
            delete[] colors;
            continue;
        }
        Text* t = dynamic_cast<Text*>(i->second);
        if (t != 0) {
            unsigned int fontSize = 11;
            if (t->xScaleDominated(*fontLoader)) {
                fontSize *= t->width / 0.108 * currentGeometry.width / (currentXMax - currentXMin);
            } else {
                //! @todo find a case where text is height dominated and insert a fraction to t->height
                fontSize *= currentGeometry.height / (currentYMax - currentYMin);
            }

            setFont(t->getFontName(), fontSize);
            setColor(t->getColor());
            writeTextCentered(t->x + 0.5 * t->width, t->y + 0.5 * t->height, t->text);
            continue;
        }
    }
    writeFooter();
}

void PostscriptPrinter::writeHeader()
{
    out << "%!PS-Adobe-3.0" << std::endl;
    out << "%%BoundingBox 0 0 " << (pixelX+2*pixelBoundary) << " " << (pixelY+2*pixelBoundary) << std::endl;
    out << "%%PageOrientation: " << (orientation == Portrait ? "Portrait" : "Landscape") << std::endl;
    out << "%%PageBoundingBox 0 0 " << (pixelX+2*pixelBoundary) << " " << (pixelY+2*pixelBoundary) << std::endl;
    out << "%%EndPageSetup" << std::endl;

    out << "/myline { gsave newpath moveto lineto stroke grestore } def" << std::endl;
    out << "/mypoint { gsave newpath moveto gsave currentpoint lineto 1 setlinecap stroke grestore grestore } def" << std::endl;
    out << "/mybox { gsave 1 dict begin /n exch def newpath moveto 1 1 n { /i exch def lineto } for closepath gsave fill grestore end grestore } def" << std::endl;
    out << "/mytext { gsave moveto show grestore } def" << std::endl;
    out << "/mytextcentered { gsave 3 dict begin /y exch def /x exch def /t exch def x y moveto t dup true charpath pathbbox 3 -1 roll sub 2 div neg 3 1 roll sub 2 div exch x y moveto rmoveto show end grestore } def" << std::endl;

    double s = 1.0 / sizefactor;
    out << s << " " << s << " scale" << std::endl;
}

void PostscriptPrinter::writeFooter()
{
    out << "showpage" << std::endl;
    out << "%%EOF" << std::endl;
}

void PostscriptPrinter::setFont(std::string fontname, unsigned int size)
{
    fontname[0] = toupper(fontname[0]);
    if (lastFont == nullptr || fontname != *lastFont || lastFontSize == 0.0 || size != lastFontSize) {
        out << "/" << fontname << " findfont " << size * sizefactor << " scalefont setfont" << std::endl;
        delete lastFont;
        lastFont = new std::string(fontname);
        lastFontSize = size;
    }
}

void PostscriptPrinter::setColor(const Color& color)
{
    if (lastColor == nullptr || color != *lastColor) {
        out << color.r << " " << color.g << " " << color.b << " setrgbcolor" << std::endl;
        delete lastColor;
        lastColor = new Color(color);
    }
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

void PostscriptPrinter::draw(int n, double const* x, double const* y, Color const* colors)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            setColor(colors[i*n+j]);

            double dxp = 0.5 * (x[j] - (j == 0 ? currentXMin : x[j-1]));
            double dyp = 0.5 * (y[i] - (i == 0 ? currentYMin : y[i-1]));
            double dxn = 0.5 * ((j == n-1 ? currentXMax : x[j+1]) - x[j]);
            double dyn = 0.5 * ((i == n-1 ? currentYMax : y[i+1]) - y[i]);

            fillShape({x[j]-dxp, x[j]-dxp, x[j]+dxn, x[j]+dxn}, {y[i]-dyp, y[i]+dyn, y[i]+dyn, y[i]-dyp});
        }
    }
}

void PostscriptPrinter::drawLine(double x1, double y1, double x2, double y2)
{
    Pixel p1 = transformCoordinates(x1, y1), p2 = transformCoordinates(x2, y2);
    out << p1.first << " " << p1.second << " " << p2.first << " " << p2.second << " myline" << std::endl;
}

void PostscriptPrinter::drawPoint(double x, double y)
{
    Pixel p = transformCoordinates(x, y);
    out << p.first << " " << p.second << " mypoint" << std::endl;
}

void PostscriptPrinter::fillShape(std::vector<double> x, std::vector<double> y)
{
    unsigned int n = (x.size() < y.size() ? x.size() : y.size());
    if (n > 0) {
        for (unsigned int i = 0; i < n; i++) {
            Pixel p = transformCoordinates(x[i], y[i]);
            out << p.first << " " << p.second << " ";
        }
        out << (n-1) << " mybox" << std::endl;
    }
}

void PostscriptPrinter::writeText(double x, double y, std::string const& text)
{
    Pixel p = transformCoordinates(x, y);
    out << "(" << text << ") " << p.first << " " << p.second << " mytext" << std::endl;
}

void PostscriptPrinter::writeTextCentered(double x, double y, std::string const& text)
{
    Pixel p = transformCoordinates(x, y);
    out << "(" << text << ") " << p.first << " " << p.second << " mytextcentered" << std::endl;
}
}
