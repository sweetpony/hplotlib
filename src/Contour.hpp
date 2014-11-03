#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include <cmath>

#include "GL/glld.h"

#include "Plot.hpp"
#include "Statistics.hpp"
#include "ColorTable.hpp"

namespace hpl
{
class Contour : public Plot {
public:
    Contour(int n, double const* x, double const* y, double const* z);
    virtual ~Contour();

    template<ColorTable::Tables t>
    inline void setColorTable(unsigned int length) {
        colorTable = ColorTable::getPredefinedTable<t>(length);
        recalc = true;
        changed.invoke();
    }

    virtual void init(GLuint lineprogram, GLuint);
    virtual void destroy();
    virtual void draw(float const* mvp);

private:
    void updateTexture();

    int n;
    float rectCorners[16];
    double xmin, ymin, xmax, ymax, zmin, zmax;
    double const* z;
    float* data = nullptr;

    bool recalc = false;

    GLuint textureid, mapBuffer, program;
    GLint pos, uv, rect, colorMap, linemvp;

    ColorTable colorTable = ColorTable::getPredefinedTable<ColorTable::RainbowBlack>(256);
};
}

#endif // CONTOUR_HPP
