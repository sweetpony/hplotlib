#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include "Drawable.hpp"
#include "Statistics.hpp"
#include "ColorTable.hpp"

namespace hpl
{
//! @todo need to redsign slightly to make use of xlog and ylog
class Contour : public Drawable
{

public:
    Contour(int n, double const* x, double const* y, double const* z) : Drawable(Type_Texture), n(n), x(x), y(y), z(z) {
        zmin = hpl::min(n*n, z);
        zmax = hpl::max(n*n, z);
    }
    virtual ~Contour() {}

    inline virtual void setLimits(double xmin, double ymin, double xmax, double ymax, double zmin, double zmax) {
        this->xmin = xmin;
        this->ymin = ymin;
        this->xmax = xmax;
        this->ymax = ymax;
        this->zmin = zmin;
        this->zmax = zmax;
        if (recalculateOnLimitChangeNeeded()) {
            recalculateData();
        } else {
            changed.invoke(plotId);
        }
    }
    inline double getZmin() const {
        return zmin;
    }
    inline double getZmax() const {
        return zmax;
    }

    template<ColorTable::Tables t>
    inline void setColorTable(unsigned int length) {
        colorTable = ColorTable::getPredefinedTable<t>(length);
        changed.invoke(plotId);
    }
    inline const ColorTable& getColorTable() const {
        return colorTable;
    }

    const int n;
    const double* x, * y, * z;

protected:
    inline virtual void recalculateData() {}
    inline virtual bool recalculateOnLimitChangeNeeded() {
        return false;
    }

    double zmin, zmax;
    ColorTable colorTable = ColorTable::getPredefinedTable<ColorTable::RainbowBlack>(256);
};
}

#endif // CONTOUR_HPP
