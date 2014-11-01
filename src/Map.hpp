#ifndef MAP_HPP
#define MAP_HPP

#include "GL/glld.h"

#include "Plot.hpp"
#include "Statistics.hpp"

namespace hpl
{
class Map : public Plot {
public:
    Map(int n, double const* x, double const* y, double const* z);
    virtual ~Map();

    virtual void init(GLuint lineprogram, GLuint);
    virtual void destroy();
    virtual void draw(float const* mvp);

private:
    int n;
    float* rectCorners = nullptr;
    double xmin, ymin, xmax, ymax, zmin, zmax;

    GLuint textureid;

    GLuint mapBuffer;
    GLuint program;

    GLint pos, uv, rect, colorMap, linemvp;
};
}

#endif // MAP_HPP
