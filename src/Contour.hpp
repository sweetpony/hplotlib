#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include <cmath>

#include "Drawable.hpp"
#include "Statistics.hpp"
#include "ColorTable.hpp"

namespace hpl
{
//! @todo need to redsign slightly to make use of xlog and ylog
class Contour : public Drawable
{

public:
    Contour(int n, double const* x, double const* y, double const* z, const Limits& limits) :
        Drawable(Type_Texture, limits), n(n), x(x), y(y), z(z) {
        zmin = hpl::min(n*n, z);
        zmax = hpl::max(n*n, z);
    }
    virtual ~Contour() {}

    inline virtual void setLimits(double zmin, double zmax) {
        this->zmin = zmin;
        this->zmax = zmax;
        recalculateData();
    }
    inline double getZmin() const {
        return zmin;
    }
    inline double getZmax() const {
        return zmax;
    }

    Color* getColors() const;
    Color getColorAtIndex(int i) const;

    template<ColorTable::Tables t>
    inline void setColorTable(unsigned int length) {
        colorTable = ColorTable::getPredefinedTable<t>(length);
        changed.invoke(plotId);
    }
    inline const ColorTable& getColorTable() const {
        return colorTable;
    }

    inline virtual void recalculateData() {}

    const int n;
    const double* x, * y, * z;

protected:
    double zmin, zmax;
    ColorTable colorTable = ColorTable::getPredefinedTable<ColorTable::RainbowBlack>(256);
};
}

#endif // CONTOUR_HPP
