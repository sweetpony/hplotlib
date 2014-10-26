#ifndef SCATTERPLOT_HPP
#define SCATTERPLOT_HPP


#include "Plot.hpp"
#include "Points.hpp"

namespace hpl
{
class ScatterPlot : public Plot
{

public:
    ScatterPlot();
    ~ScatterPlot();

    virtual void addPoints(Points* lines);

    virtual void setPointColor(unsigned int id, const Color& c);

};
}

#endif // SCATTERPLOT_HPP
