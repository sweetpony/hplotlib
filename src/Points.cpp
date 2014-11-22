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

    switch(symbol) {
    case Dot:
        METH_PB(0.0, 0.0);
        break;
    case Plus:
        METH_PB(0.0, 0.5*size);
        METH_PB(0.0, -0.5*size);
        METH_PB(0.5*size, 0.0);
        METH_PB(-0.5*size, 0.0);
        break;
    case Cross:
        METH_PB(0.5*size, 0.5*size);
        METH_PB(-0.5*size, -0.5*size);
        METH_PB(0.5*size, -0.5*size);
        METH_PB(-0.5*size, 0.5*size);
        break;
    case Asterisk:
        METH_PB(0.0, 0.5*size);
        METH_PB(0.0, -0.5*size);
        METH_PB(0.5*size, 0.0);
        METH_PB(-0.5*size, 0.0);
        METH_PB(0.5*size, 0.5*size);
        METH_PB(-0.5*size, -0.5*size);
        METH_PB(0.5*size, -0.5*size);
        METH_PB(-0.5*size, 0.5*size);
        break;
    case Circle:
    case FilledCircle:
        for (unsigned int i = 0; i < maxSymbolVertices; i++) {
            METH_PB(std::cos(i * 2 * M_PI / maxSymbolVertices), std::sin(i * 2 * M_PI / maxSymbolVertices));
            METH_PB(std::cos((i+1) * 2 * M_PI / maxSymbolVertices), std::sin((i+1) * 2 * M_PI / maxSymbolVertices));
        }
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
    case Plus:
    case Cross:
    case Asterisk:
    case Circle:
    case FilledCircle:
        type = Type_Lines;
        break;
    }
}
}
