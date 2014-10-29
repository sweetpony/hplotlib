#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "Geometry.hpp"
#include "Legend.hpp"
#include "Delegate.hpp"
#include "Plot.hpp"
#include "Registry.hpp"

#include <vector>

namespace hpl
{
class Layout
{
public:
	typedef IDBase<Layout> ID;

    Layout() {}
    virtual ~Layout() {}

    inline void setXMargins(double left, double right) {
        margins[0] = left;
        margins[1] = right;
    }

    inline void setYMargins(double top, double bottom) {
        margins[2] = top;
        margins[3] = bottom;
    }

    virtual void recalculate(std::vector<Geometry>& geometries) = 0;

    Delegate<> changed;
    
	inline ID id() const { return layoutid; }
    inline void setId(ID id) { layoutid = id; }
    
protected:
    //! @todo how are margins exactly defined here? use them
    double margins[4]; //left, right, top, bottom
    ID layoutid;
};
}

#endif // LAYOUT_HPP
