#ifndef LINEPLOT_HPP
#define LINEPLOT_HPP

#include "Plot.hpp"
#include "Lines.hpp"

namespace hpl
{
class LinePlot : public Plot
{

public:
    LinePlot();
    virtual ~LinePlot();

    virtual void addLines(Lines* lines);

    virtual void setLineColor(unsigned int id, const Color& c);

};
}

#endif // LINEPLOT_HPP
