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
    //! @todo use flags
    setMargins(0.12f, 0.12f, 0.08f, 0.08f);
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

}
