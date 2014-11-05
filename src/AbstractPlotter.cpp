#include "AbstractPlotter.hpp"

namespace hpl {
AbstractPlotter::AbstractPlotter(const Registry<Plot>& plots) : plots(plots)
{
}

AbstractPlotter::~AbstractPlotter()
{
}
}
