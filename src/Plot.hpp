#ifndef PLOT_HPP
#define PLOT_HPP

#include <vector>
#include "Legend.hpp"
#include "Color.hpp"
#include "Delegate.hpp"

namespace hpl
{
class Plot
{

public:
    Plot();
    virtual ~Plot();

    virtual inline std::vector<PlotPart*> getPlotParts() {
        return parts;
    }

    virtual void init(GLuint lineprogram, GLuint textprogram);
    virtual void destroy();
    virtual void draw(float const* mvp);

    virtual void addLegend(Legend* legend);
    Legend* getLegend();

    virtual void setLegendColor(const Color& c);
    
    Delegate<> changed;

protected:
    Legend* legend;
    std::vector<PlotPart*> parts;
};
}

#endif // PLOT_HPP
