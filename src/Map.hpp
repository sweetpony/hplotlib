#ifndef MAP_HPP
#define MAP_HPP

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

    GLuint mapBuffer;
    GLuint program;

    GLint pos, rect, colorMap, linemvp;
};
}

#endif // MAP_HPP
