#ifndef POINTS_H
#define POINTS_H

#include "PlotPart.hpp"
#include "Statistics.hpp"

namespace hpl
{
class Points : public PlotPart
{

public:
    Points(int n, double const* x, double const* y, Geometry* geometry);
    virtual ~Points();

    float* getX() const;
    float* getY() const;

    inline int getN() const {
        return n;
    }

    virtual void init(GLuint lineprogram, GLuint);
    virtual void destroy();
    virtual void draw(float const* mvp);

private:
    int n;
    float* points = nullptr;
    double xmin, ymin, xmax, ymax;

    GLuint pointBuffer;
    GLuint program;

    GLint pos, rect, color, pointmvp;

};
}

#endif // POINTS_H
