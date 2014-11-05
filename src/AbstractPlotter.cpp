#include "AbstractPlotter.hpp"

namespace hpl {
AbstractPlotter::AbstractPlotter(const Registry<Drawable>& plots) : plots(plots)
{
}

AbstractPlotter::~AbstractPlotter()
{
}
}
