#include "CooSyGeometry.hpp"

namespace hpl
{
CooSyGeometry::CooSyGeometry()
{
    setPlotGeometryFromBase();
}

CooSyGeometry::CooSyGeometry(const Geometry& baseGeometry) : baseGeometry(baseGeometry)
{
    setPlotGeometryFromBase();
}

void CooSyGeometry::setGeometry(Geometry geometry)
{
    this->baseGeometry = geometry;

    setPlotGeometryFromBase();

    changed.invoke(Drawable::ID());
}

void CooSyGeometry::setMargins(float leftOffset, float rightOffset, float bottomOffset, float topOffset)
{
    this->leftOffset = leftOffset;
    this->rightOffset = rightOffset;
    this->bottomOffset = bottomOffset;
    this->topOffset = topOffset;

    setPlotGeometryFromBase();

    changed.invoke(Drawable::ID());
}

void CooSyGeometry::resetMargins()
{
    float left = resetXMargins(flags[XTicksPrimary], flags[XTitlePrimary]);
    float right = resetXMargins(flags[XTicksSecondary], flags[XTitleSecondary]);
    if (left + right > maxResetXMargin) {
        float frac = left / (left + right);
        left = frac * maxResetXMargin;
        right = (1.0f - frac) * maxResetXMargin;
    }

    float bottom = resetYMargins(flags[YTicksPrimary], flags[YTitlePrimary], false);
    float top = resetYMargins(flags[YTicksSecondary], flags[YTitleSecondary], flags[PlotTitle]);
    if (bottom + top > maxResetYMargin) {
        float frac = bottom / (bottom + top);
        bottom = frac * maxResetYMargin;
        top = (1.0f - frac) * maxResetYMargin;
    }

    setMargins(left, right, bottom, top);
}

void CooSyGeometry::setPlotGeometryFromBase()
{
    Geometry geom(baseGeometry);

    geom.leftOffset += leftOffset * geom.width;
    geom.topOffset += bottomOffset * geom.height;
    geom.width *= (1.0 - leftOffset - rightOffset);
    geom.height *= (1.0 - bottomOffset - topOffset);

    plotGeometry = baseGeometry;
}

float CooSyGeometry::resetXMargins(bool ticks, bool title)
{
    if (ticks) {
        if (title) {
            return 0.2f;
        } else {
            return 0.12f;
        }
    } else {
        if (title) {
            return 0.12f;
        } else {
            return 0.0f;
        }
    }
}

float CooSyGeometry::resetYMargins(bool ticks, bool title, bool plotTitle)
{
    if (ticks) {
        if (title) {
            if (plotTitle) {
                return 0.2f;
            } else {
                return 0.15f;
            }
        } else {
            if (plotTitle) {
                return 0.15f;
            } else {
                return 0.08f;
            }
        }
    } else {
        if (title) {
            if (plotTitle) {
                return 0.15f;
            } else {
                return 0.08f;
            }
        } else {
            if (plotTitle) {
                return 0.08f;
            } else {
                return 0.0f;
            }
        }
    }
}

}
