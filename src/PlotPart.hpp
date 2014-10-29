/*
 * Plot.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_PLOTPART_HPP
#define HPLOTLIB_PLOTPART_HPP

#include <GL/glld.h>
#include "Color.hpp"
#include "Geometry.hpp"
#include "IDBase.hpp"

namespace hpl
{
class PlotPart {

public:
    typedef IDBase<PlotPart> ID;

    PlotPart() {}
    virtual ~PlotPart() {}

	virtual void init(GLuint lineprogram, GLuint textprogram) = 0;
	virtual void destroy() = 0;
	virtual void draw(float const* mvp) = 0;

    virtual inline void setColor(const Color& c) {
        drawColor = c;
    }
    
	inline void setGeometry(Geometry const& geom) {
		geometry = geom;
    }

    inline ID id() const { return plotPartId; }
    inline void setId(ID id) { plotPartId = id; }
	
protected:
    Color drawColor = Color(0.0f, 0.0f, 0.0f);
    Geometry geometry;
    ID plotPartId;
};
}

#endif

