#ifndef PLOT_HPP
#define PLOT_HPP

#include "Color.hpp"
#include "Delegate.hpp"
#include "IDBase.hpp"
#include "Geometry.hpp"
#include "Registry.hpp"

#include <GL/glld.h>

namespace hpl
{
class Layout;
class Plot
{
public:
	typedef IDBase<Plot> ID;

    Plot() {}
    virtual ~Plot() {}

	virtual void init(GLuint lineprogram, GLuint textprogram) = 0;
	virtual void destroy() = 0;
	virtual void draw(float const* mvp) = 0;

    virtual inline void setColor(const Color& c) {
        drawColor = c;
        changed.invoke();
    }
    
	inline void setGeometry(Geometry const& geom) {
		geometry = geom;
		changed.invoke();
    }
    
    Delegate<> changed;
    
    inline ID id() const { return plotId; }
    inline void setId(ID id) { plotId = id; }

protected:
    Color drawColor = Color(0.0f, 0.0f, 0.0f);
    Geometry geometry;
    ID plotId;
};
}

#endif // PLOT_HPP
