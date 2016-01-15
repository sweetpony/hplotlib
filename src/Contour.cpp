#include "Contour.hpp"

namespace hpl {

Color* Contour::getColors() const
{
    Color* colors = new Color[n*n];

    for (int i = 0; i < n*n; ++i) {
        colors[i] = getColorAtIndex(i);
    }

    return colors;
}

Color Contour::getColorAtIndex(int i) const
{
    double zi = (z[i] - zmin) / (zmax - zmin);

    int imin = static_cast<unsigned int>(zi * colorTable.num);
    if (imin < 0) {
        imin = 0;
    } else if (imin >= colorTable.num) {
        imin = colorTable.num - 1;
    }

    int imax = static_cast<unsigned int>(ceil(zi * colorTable.num));
    if (imax < 0) {
        imax = 0;
    } else if (imax >= colorTable.num) {
        imax = colorTable.num - 1;
    }

    double delta = zi * colorTable.num - imin;

    float r = colorTable.r[imin] * delta + colorTable.r[imax] * (1.0 - delta);
    float g = colorTable.g[imin] * delta + colorTable.g[imax] * (1.0 - delta);
    float b = colorTable.b[imin] * delta + colorTable.b[imax] * (1.0 - delta);

    return Color(r, g, b);
}

}
