#include "Map.hpp"

namespace hpl
{
Map::Map(int n, double const* x, double const* y, double const* z)
    : n(n)
{
    xmin = hpl::min(n, x);
    xmax = hpl::max(n, x);
    ymin = hpl::min(n, y);
    ymax = hpl::max(n, y);

    /*rectCorners = new float[2*n];
    for (int i = 0; i < n; ++i) {
        rectCorners[(i << 1)] = (x[i] - xmin) / (xmax - xmin);
        rectCorners[(i << 1) + 1] = (y[i] - ymin) / (ymax - ymin);
    }*/

    n = 2;
    rectCorners = new float[12];
    rectCorners[0] = 0.78f; rectCorners[1] = 0.78f;
    rectCorners[2] = 0.78f; rectCorners[3] = 0.11f;
    rectCorners[4] = 0.83f; rectCorners[5] = 0.78f;
    rectCorners[6] = 0.83f; rectCorners[7] = 0.11f;
    rectCorners[8] = 0.88f; rectCorners[9] = 0.78f;
    rectCorners[10] = 0.88f; rectCorners[11] = 0.11f;
}

Map::~Map()
{
    delete[] rectCorners;
}

void Map::init(GLuint lineprogram, GLuint)
{
    program = lineprogram;

    glGenBuffers(1, &lineBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
    glBufferData(GL_ARRAY_BUFFER, (4 + 8 * (n - 1)) * sizeof(float), rectCorners, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    pos = glGetAttribLocation(program, "Position");
    rect = glGetUniformLocation(program, "Rect");
    color = glGetUniformLocation(program, "Color");
    linemvp = glGetUniformLocation(program, "MVP");
}

void Map::destroy()
{
    glDeleteBuffers(1, &lineBuffer);
}

void Map::draw(float const* mvp)
{
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
    glVertexAttribPointer(
        pos,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (GLvoid const*) 0
    );
    glEnableVertexAttribArray(pos);
    glUniform4f(rect, geometry.leftOffset, geometry.topOffset, geometry.width, geometry.height);
    glUniform3f(color, drawColor.r, drawColor.g, drawColor.b);
    glUniformMatrix3fv(linemvp, 1, GL_FALSE, mvp);

    glDrawArrays(GL_QUAD_STRIP, 0, 2 + 4 * (n - 1));
    glDisableVertexAttribArray(pos);
}
}
