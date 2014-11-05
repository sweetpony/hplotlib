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
class Drawable
{
public:
	typedef IDBase<Drawable> ID;

    Drawable() {}
    virtual ~Drawable() {}

    inline void setLimits(double xmin, double ymin, double xmax, double ymax) {
        this->xmin = xmin;
        this->ymin = ymin;
        this->xmax = xmax;
        this->ymax = ymax;
        changed.invoke();
    }

    inline double getXmin() const {
        return xmin;
    }
    inline double getYmin() const {
        return ymin;
    }
    inline double getXmax() const {
        return xmax;
    }
    inline double getYmax() const {
        return ymax;
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
    inline Geometry getGeometry() const {
        return geometry;
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
