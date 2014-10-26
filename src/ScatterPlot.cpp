#include "ScatterPlot.hpp"

namespace hpl
{
ScatterPlot::ScatterPlot() : Plot()
{
}

ScatterPlot::~ScatterPlot()
{
}

void ScatterPlot::addPoints(Points* points)
{
    parts.push_back(points);
    changed.invoke();
}

void ScatterPlot::setPointColor(unsigned int id, const Color& c)
{
    parts.at(id)->setColor(c);
    changed.invoke();
}
}
