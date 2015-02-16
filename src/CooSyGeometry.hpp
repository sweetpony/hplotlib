#ifndef COOSYGEOMETRY_HPP
#define COOSYGEOMETRY_HPP

#include "Geometry.hpp"
#include "Delegate.hpp"
#include "Drawable.hpp"
#include "Flags.hpp"

namespace hpl
{
class CooSyGeometry
{

public:

    enum GeometryFlags {
        XTicksPrimary = 1 << 0,
        XTitlePrimary = 1 << 1,
        XTicksSecondary = 1 << 2,
        XTitleSecondary = 1 << 3,
        YTicksPrimary = 1 << 4,
        YTitlePrimary = 1 << 5,
        YTicksSecondary = 1 << 6,
        YTitleSecondary = 1 << 7,
        PlotTitle = 1 << 8
    };

    CooSyGeometry();
    CooSyGeometry(const Geometry& baseGeometry);

    void setGeometry(Geometry geometry);

    void setMargins(float leftOffset, float rightOffset, float bottomOffset, float topOffset);
    void resetMargins();

    inline const Geometry& getBaseGeometry() const {
        return baseGeometry;
    }
    inline const Geometry& getPlotGeometry() const {
        return plotGeometry;
    }

    MultiDelegate<Drawable::ID> changed;
    Flags<GeometryFlags> flags =  Flags<GeometryFlags>(XTicksPrimary, YTicksPrimary);
    float maxResetXMargin = 0.2f, maxResetYMargin = 0.2f;

private:
    void setPlotGeometryFromBase();

    float resetXMargins(bool ticks, bool title);
    float resetYMargins(bool ticks, bool title, bool plotTitle);

    Geometry baseGeometry, plotGeometry;
    float leftOffset = 0.12f, rightOffset = 0.0f, bottomOffset = 0.08f, topOffset = 0.0f;

};
}

#endif // COOSYGEOMETRY_HPP
