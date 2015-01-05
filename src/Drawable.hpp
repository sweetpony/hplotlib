#ifndef PLOT_HPP
#define PLOT_HPP

#include "Color.hpp"
#include "Delegate.hpp"
#include "IDBase.hpp"
#include "Geometry.hpp"
#include "Registry.hpp"
#include "Statistics.hpp"
#include "Limits.hpp"

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

    Drawable(Type type, const Limits& limits) : limits(limits), type(type) {}
    virtual ~Drawable() {}

    inline double xmin() const {
        return limits.xmin();
    }
    inline double ymin() const {
        return limits.ymin();
    }
    inline double xmax() const {
        return limits.xmax();
    }
    inline double ymax() const {
        return limits.ymax();
    }

    virtual void setLog(bool xlog, bool ylog) {
        this->xlog = xlog;
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

    virtual void recalculateData() = 0;

protected:
    const Limits& limits;
    bool xlog = false, ylog = false;
    Geometry geometry;
    ID plotId;
    Type type;
};
}

#endif // PLOT_HPP
