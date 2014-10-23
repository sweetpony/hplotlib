#include "Lineplot.hpp"

namespace hpl
{
LinePlot::LinePlot()
{
}

LinePlot::~LinePlot()
{
}

void LinePlot::addLines(Lines* lines)
{
    parts.push_back(lines);
}
}
