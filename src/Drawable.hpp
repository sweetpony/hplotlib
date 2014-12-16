#ifndef PLOT_HPP
#define PLOT_HPP

#include "Color.hpp"
#include "Delegate.hpp"
#include "IDBase.hpp"
#include "Geometry.hpp"
#include "Registry.hpp"
#include "Statistics.hpp"

#include "GL/glld.h"

namespace hpl
{
class Layout;
class Drawable
{
public:
	typedef IDBase<Drawable> ID;

    enum Type {
        Type_Lines,
        Type_LineStrips,
        Type_Points,
        Type_Texture
    };

    Drawable(Type type) : type(type) {}
    virtual ~Drawable() {}

    inline void setLimits(double xmin, double ymin, double xmax, double ymax) {
        this->xmin = xmin;
        this->ymin = ymin;
        this->xmax = xmax;
        this->ymax = ymax;
        if (recalculateOnLimitChangeNeeded()) {
            recalculateData();
        } else {
            changed.invoke(plotId);
        }
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

    virtual void setLog(bool xlog, bool ylog) {
        this->xlog  = xlog;
        this->ylog = ylog;
        recalculateData();
    }
    
	inline void setGeometry(Geometry const& geom) {
		geometry = geom;
        changed.invoke(plotId);
    }
    inline Geometry getGeometry() const {
        return geometry;
    }

    inline Type getDataType() const {
        return type;
    }
    
    Delegate<ID> changed;
    
    inline ID id() const { return plotId; }
    inline void setId(ID id) { plotId = id; }

protected:
    virtual void recalculateData() = 0;
    virtual bool recalculateOnLimitChangeNeeded() = 0;

    double xmin, ymin, xmax, ymax;
    bool xlog = false, ylog = false;
    Geometry geometry;
    ID plotId;
    Type type;
};
}

#endif // PLOT_HPP
