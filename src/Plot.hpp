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
	typedef unsigned ID;

    Plot();
    virtual ~Plot();

    virtual inline std::vector<PlotPart*> getPlotParts() {
        return parts;
    }

    virtual void init(GLuint lineprogram, GLuint textprogram);
    virtual void destroy();
    virtual void draw(float const* mvp);

    virtual void addLegend(Legend* legend);
    inline Legend* getLegend() {
        return legend;
    }

    virtual void setLegendColor(const Color& c);    
    Delegate<> changed;
    
    inline void setId(ID id) { plotid = id; }	
	inline ID id() const { return plotid; }

protected:
    Legend* legend;
    std::vector<PlotPart*> parts;
    ID plotid;
};
}

#endif // PLOT_HPP
