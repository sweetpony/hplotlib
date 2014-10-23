#include "LinePlot.hpp"

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

void LinePlot::setLineColor(unsigned int id, const Color& c)
{
    parts.at(id)->setColor(c);
}
}
