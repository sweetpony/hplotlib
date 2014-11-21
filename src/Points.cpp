#include "Points.hpp"

#include <cmath>
#define M_PI		3.14159265358979323846

namespace hpl
{

#define METH_START(T) \
template<> \
std::vector<std::pair<double, double> > Points::getSymbolVertices<Points::T>() const { \
    std::vector<std::pair<double, double> > vert;

#define METH_PB(X, Y) \
    vert.push_back(std::pair<double, double>(X, Y));

#define METH_END \
    return vert; \
}



METH_START(Dot)
METH_PB(0.0, 0.0)
METH_END

METH_START(Plus)
METH_PB(0.0, 0.5*size)
METH_PB(0.0, -0.5*size)
METH_PB(0.5*size, 0.0)
METH_PB(-0.5*size, 0.0)
METH_END

METH_START(Cross)
METH_PB(0.5*size, 0.5*size)
METH_PB(-0.5*size, -0.5*size)
METH_PB(0.5*size, -0.5*size)
METH_PB(-0.5*size, 0.5*size)
METH_END

METH_START(Asterisk)
METH_PB(0.0, 0.5*size)
METH_PB(0.0, -0.5*size)
METH_PB(0.5*size, 0.0)
METH_PB(-0.5*size, 0.0)
METH_PB(0.5*size, 0.5*size)
METH_PB(-0.5*size, -0.5*size)
METH_PB(0.5*size, -0.5*size)
METH_PB(-0.5*size, 0.5*size)
METH_END

METH_START(Circle)
for (unsigned int i = 0; i < maxSymbolVertices; i++) {
    METH_PB(std::cos(i * 2 * M_PI / maxSymbolVertices), std::sin(i * 2 * M_PI / maxSymbolVertices))
}
METH_END

}
