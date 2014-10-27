#include "Points.hpp"

namespace hpl
{
Points::Points(int n, double const* x, double const* y, Geometry* geometry) : PlotPart(geometry), n(n)
{
    xmin = hpl::min(n, x);
    xmax = hpl::max(n, x);
    ymin = hpl::min(n, y);
    ymax = hpl::max(n, y);

    points = new float[2*n];
    for (int i = 0; i < n; ++i) {
        points[(i << 1)] = (x[i] - xmin) / (xmax - xmin);
        points[(i << 1) + 1] = (y[i] - ymin) / (ymax - ymin);
    }
}

Points::~Points()
{
}

float* Points::getX() const
{
    float* x = new float[n];
    for (unsigned int i = 0; i < n; i++) {
        x[i] = points[i << 1] * geometry->width + geometry->leftOffset;
    }
    return x;
}

float* Points::getY() const
{
    float* y = new float[n];
    for (unsigned int i = 0; i < n; i++) {
        y[i] = points[(i << 1) + 1] * geometry->height + geometry->topOffset;
    }
    return y;
}

void Points::init(GLuint lineprogram, GLuint)
{
    program = lineprogram;

    glGenBuffers(1, &pointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * n * sizeof(float), points, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    pos = glGetAttribLocation(program, "Position");
    rect = glGetUniformLocation(program, "Rect");
    color = glGetUniformLocation(program, "Color");
    pointmvp = glGetUniformLocation(program, "MVP");
}

void Points::destroy()
{
    glDeleteBuffers(1, &pointBuffer);
}

void Points::draw(float const* mvp)
{
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, pointBuffer);
    glVertexAttribPointer(
        pos,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (GLvoid const*) 0
    );
    glEnableVertexAttribArray(pos);
    glUniform4f(rect, geometry->leftOffset, geometry->topOffset, geometry->width, geometry->height);
    glUniform3f(color, drawColor.r, drawColor.g, drawColor.b);
    glUniformMatrix3fv(pointmvp, 1, GL_FALSE, mvp);

    glDrawArrays(GL_POINTS, 0, n);
    glDisableVertexAttribArray(pos);
}
}
