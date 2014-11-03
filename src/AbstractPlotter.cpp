#include "AbstractPlotter.hpp"

namespace hpl {
AbstractPlotter::AbstractPlotter(const PaintServer& paintServer) : paintServer(paintServer)
{
}

AbstractPlotter::~AbstractPlotter()
{
}
}
