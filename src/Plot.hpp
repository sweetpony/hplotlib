#ifndef PLOT_HPP
#define PLOT_HPP

#include "Color.hpp"
#include "Delegate.hpp"
#include "IDBase.hpp"
#include "Geometry.hpp"
#include "Registry.hpp"

#include "GL/glld.h"

namespace hpl
{
class Layout;
class Plot
{
public:
	typedef IDBase<Plot> ID;

    Plot() {}
    virtual ~Plot() {}

    inline void setLimits(double xmin, double ymin, double xmax, double ymax) {
        this->xmin = xmin;
        this->ymin = ymin;
        this->xmax = xmax;
        this->ymax = ymax;
        changed.invoke();
    }

    virtual inline void setColor(const Color& c) {
        drawColor = c;
        changed.invoke();
    }
    virtual inline Color getColor() const {
        return drawColor;
    }
    
	inline void setGeometry(Geometry const& geom) {
		geometry = geom;
		changed.invoke();
    }
    
    Delegate<> changed;
    
    inline ID id() const { return plotId; }
    inline void setId(ID id) { plotId = id; }

protected:
    double xmin, ymin, xmax, ymax;
    Color drawColor = Color(0.0f, 0.0f, 0.0f);
    Geometry geometry;
    ID plotId;
};
}

#endif // PLOT_HPP
