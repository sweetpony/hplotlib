#include "Points.hpp"

#include <cmath>

#define M_PI		3.14159265358979323846
#define METH_PB(X, Y) \
    vert.push_back(std::pair<double, double>(X, Y))

namespace hpl
{
void Points::setSymbol(Symbol s)
{
    if (s != symbol) {
        symbol = s;
        delete points;

        std::vector<std::pair<double, double> > vert = getSymbolVertices();
        if (vert.size() == 1 && vert[0].first == 0.0 && vert[1].second == 0.0) {
            points = new SimplePoints(_n, _x, _y, false);
        } else {
            int n = _n * vert.size();
            double* x = new double[n];
            double* y = new double[n];

            //! @todo Actually would need aspect ratio of current box, which is geometry & aspect ratio of window
            double facx = 0.005;
            double facy = facx * (ymax - ymin) / (xmax - xmin);

            for (int i = 0, k = 0; i < _n; ++i) {
                for (unsigned int j = 0; j < vert.size(); ++j, ++k) {
                    x[k] = _x[i] + facx * vert[j].first;
                    y[k] = _y[i] + facy * vert[j].second;
                }
            }

            points = new SimplePoints(n, x, y, true);
        }
        setTypeForSymbol();

        changed.invoke(plotId);
    }
}

std::vector<std::pair<double, double> > Points::getSymbolVertices() const
{
    std::vector<std::pair<double, double> > vert;
    double l = 0.5*size;

    switch(symbol) {
    case Dot:
        METH_PB(0.0, 0.0);
        break;
    case Plus:
        METH_PB(0.0, l);
        METH_PB(0.0, -l);
        METH_PB(l, 0.0);
        METH_PB(-l, 0.0);
        break;
    case Cross:
        METH_PB(l, l);
        METH_PB(-l, -l);
        METH_PB(l, -l);
        METH_PB(-l, l);
        break;
    case Asterisk:
        METH_PB(0.0, l);
        METH_PB(0.0, -l);
        METH_PB(l, 0.0);
        METH_PB(-l, 0.0);
        METH_PB(l, l);
        METH_PB(-l, -l);
        METH_PB(l, -l);
        METH_PB(-l, l);
        break;
    case Diamond:
    case FilledDiamond:
        METH_PB(0.0, l);
        METH_PB(l, 0.0);
        METH_PB(0.0, -l);
        METH_PB(-l, 0.0);
        METH_PB(0.0, l);
        break;
    case Circle:
    case FilledCircle:
        for (unsigned int i = 0; i < maxSymbolVertices/2; i++) {
            METH_PB(std::cos(i * 4 * M_PI / maxSymbolVertices), std::sin(i * 4 * M_PI / maxSymbolVertices));
            METH_PB(std::cos((i+1) * 4 * M_PI / maxSymbolVertices), std::sin((i+1) * 4 * M_PI / maxSymbolVertices));
        }
        break;
    case CirclePlus:
        METH_PB(0.0, l);
        METH_PB(0.0, -l);
        METH_PB(l, 0.0);
        METH_PB(-l, 0.0);
        for (unsigned int i = 0; i < (maxSymbolVertices-4)/2; i++) {
            METH_PB(std::cos(i * 4 * M_PI / (maxSymbolVertices-4)), std::sin(i * 4 * M_PI / (maxSymbolVertices-4)));
            METH_PB(std::cos((i+1) * 4 * M_PI / (maxSymbolVertices-4)), std::sin((i+1) * 4 * M_PI / (maxSymbolVertices-4)));
        }
        break;
    case CircleCross:
        METH_PB(l, l);
        METH_PB(-l, -l);
        METH_PB(l, -l);
        METH_PB(-l, l);
        for (unsigned int i = 0; i < (maxSymbolVertices-4)/2; i++) {
            METH_PB(std::cos(i * 4 * M_PI / (maxSymbolVertices-4)), std::sin(i * 4 * M_PI / (maxSymbolVertices-4)));
            METH_PB(std::cos((i+1) * 4 * M_PI / (maxSymbolVertices-4)), std::sin((i+1) * 4 * M_PI / (maxSymbolVertices-4)));
        }
        break;
    case Triangle:
    case FilledTriangle:
        METH_PB(-l, -l);
        METH_PB(0.0, l);
        METH_PB(0.0, l);
        METH_PB(l, -l);
        METH_PB(l, -l);
        METH_PB(-l, -l);
        break;
    case DownwardTriangle:
    case FilledDownwardTriangle:
        METH_PB(-l, l);
        METH_PB(0.0, -l);
        METH_PB(0.0, -l);
        METH_PB(l, l);
        METH_PB(l, l);
        METH_PB(-l, l);
        break;
    case RightwardTriangle:
    case FilledRightwardTriangle:
        METH_PB(-l, l);
        METH_PB(l, 0.0);
        METH_PB(l, 0.0);
        METH_PB(-l, -l);
        METH_PB(-l, -l);
        METH_PB(-l, l);
        break;
    case LeftwardTriangle:
    case FilledLeftwardTriangle:
        METH_PB(l, l);
        METH_PB(-l, 0.0);
        METH_PB(-l, 0.0);
        METH_PB(l, -l);
        METH_PB(l, -l);
        METH_PB(l, l);
        break;
    case Square:
    case FilledSquare:
        METH_PB(-l, l);
        METH_PB(l, l);
        METH_PB(l, l);
        METH_PB(l, -l);
        METH_PB(l, -l);
        METH_PB(-l, -l);
        METH_PB(-l, -l);
        METH_PB(-l, l);
        break;
    case Hourglass:
    case FilledHourglass:
        METH_PB(-l, -l);
        METH_PB(l, -l);
        METH_PB(l, -l);
        METH_PB(-l, l);
        METH_PB(-l, l);
        METH_PB(l, l);
        METH_PB(l, l);
        METH_PB(-l, -l);
        break;
    case Bowtie:
    case FilledBowtie:
        METH_PB(-l, -l);
        METH_PB(-l, l);
        METH_PB(-l, l);
        METH_PB(l, -l);
        METH_PB(l, -l);
        METH_PB(l, l);
        METH_PB(l, l);
        METH_PB(-l, -l);
        break;
    case VerticalBar:
    case FilledVerticalBar:
        METH_PB(-0.5*l, -l);
        METH_PB(-0.5*l, l);
        METH_PB(-0.5*l, l);
        METH_PB(0.5*l, l);
        METH_PB(0.5*l, l);
        METH_PB(0.5*l, -l);
        METH_PB(0.5*l, -l);
        METH_PB(-0.5*l, -l);
        break;
    case HorizontalBar:
    case FilledHorizontalBar:
        METH_PB(-l, -0.5*l);
        METH_PB(-l, 0.5*l);
        METH_PB(-l, 0.5*l);
        METH_PB(l, 0.5*l);
        METH_PB(l, 0.5*l);
        METH_PB(l, -0.5*l);
        METH_PB(l, -0.5*l);
        METH_PB(-l, -0.5*l);
        break;
    }

    return vert;
}

void Points::setTypeForSymbol()
{
    switch(symbol) {
    case Dot:
        type = Type_Points;
        break;
    default:
        type = Type_Lines;
        break;
    }
}
}
