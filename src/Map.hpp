#ifndef MAP_HPP
#define MAP_HPP

#include <cmath>

#include "GL/glld.h"

#include "Plot.hpp"
#include "Statistics.hpp"
#include "ColorBar.hpp"

namespace hpl
{
class Map : public Plot {
public:
    Map(int n, double const* x, double const* y, double const* z);
    virtual ~Map();

    inline void setColorBar(const ColorBar& cb) {
        colorBar = cb;
    }

    virtual void init(GLuint lineprogram, GLuint);
    virtual void destroy();
    virtual void draw(float const* mvp);

private:
    int n;
    float rectCorners[16];
    double xmin, ymin, xmax, ymax, zmin, zmax;
    double const* z;
    float* data = nullptr;

    GLuint textureid, mapBuffer, program;
    GLint pos, uv, rect, colorMap, linemvp;

    ColorBar colorBar = ColorBar::Rainbow(100);
};
}

#endif // MAP_HPP
