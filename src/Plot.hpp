#ifndef PLOT_HPP
#define PLOT_HPP

#include "Legend.hpp"
#include "Color.hpp"
#include "Delegate.hpp"
#include "IDBase.hpp"
#include "Geometry.hpp"

#include <vector>

namespace hpl
{
class Layout;
class Plot
{
public:
	typedef IDBase<Plot> ID;

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
	
	inline void setGeometry(Geometry geom) {
		legend->setGeometry(geom);
		geom.leftOffset += Legend::XOffset * geom.width;
		geom.topOffset += Legend::YOffset * geom.height;
		geom.width *= (1.0 - Legend::XOffset);
		geom.height *= (1.0 - Legend::YOffset);
		for (auto it = parts.begin(); it != parts.end(); ++it) {
			(*it)->setGeometry(geom);
		}
		changed.invoke();
	}
	
	inline ID id() const { return plotid; }
    inline void setId(ID id) { plotid = id; }

protected:
    Legend* legend;
    std::vector<PlotPart*> parts;
    ID plotid;
};
}

#endif // PLOT_HPP
