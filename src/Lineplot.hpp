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

    void addLines(Lines* lines);

};
}

#endif // LINEPLOT_HPP
