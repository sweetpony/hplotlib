#include "Contour.hpp"

namespace hpl
{
Contour::Contour(int n, double const* x, double const* y, double const* z)
    : n(n), z(z)
{
    xmin = hpl::min(n, x);
    xmax = hpl::max(n, x);
    ymin = hpl::min(n, y);
    ymax = hpl::max(n, y);
    zmin = hpl::min(n*n, z);
    zmax = hpl::max(n*n, z);

    //! @todo use x, y

    rectCorners[0] = 0.0f; rectCorners[1] = 1.0f;
    rectCorners[2] = 0.0f; rectCorners[3] = 1.0f;

    rectCorners[4] = 0.0f; rectCorners[5] = 0.0f;
    rectCorners[6] = 0.0f; rectCorners[7] = 0.0f;

    rectCorners[8] = 1.0f; rectCorners[9] = 0.0f;
    rectCorners[10] = 1.0f; rectCorners[11] = 0.0f;

    rectCorners[12] = 1.0f; rectCorners[13] = 1.0f;
    rectCorners[14] = 1.0f; rectCorners[15] = 1.0f;
}

Contour::~Contour()
{
    delete[] data;
}

void Contour::init(GLuint mapprogram, GLuint)
{
    program = mapprogram;

    glGenBuffers(1, &mapBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mapBuffer);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), rectCorners, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    pos = glGetAttribLocation(program, "Position");
    uv = glGetAttribLocation(program, "UV");
    rect = glGetUniformLocation(program, "Rect");
    colorMap = glGetUniformLocation(program, "ColorMap");
    linemvp = glGetUniformLocation(program, "MVP");

    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    delete[] data;
    data = new float[3 * n * n];
    for(int i = 0; i < n*n; ++i) {
        double zi = (z[i] - zmin) / (zmax - zmin);
        int imin = static_cast<int>(zi * colorTable.num);
        int imax = static_cast<int>(ceil(zi * colorTable.num));
        if (imax >= colorTable.num) {
            imax = colorTable.num - 1;
        }
        double delta = zi * colorTable.num - imin;
        data[i*3] = colorTable.r[imin] * delta + colorTable.r[imax] * (1.0 - delta);
        data[i*3+1] = colorTable.g[imin] * delta + colorTable.g[imax] * (1.0 - delta);
        data[i*3+2] = colorTable.b[imin] * delta + colorTable.b[imax] * (1.0 - delta);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, n, n, 0, GL_RGB, GL_FLOAT, data);
}

void Contour::destroy()
{
    glDeleteBuffers(1, &mapBuffer);
    glDeleteTextures(1, &textureid);
}

void Contour::draw(float const* mvp)
{
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, mapBuffer);
    glVertexAttribPointer(
        pos,
        2,
        GL_FLOAT,
        GL_FALSE,
        4*sizeof(GLfloat),
        (GLvoid const*) 0
    );
    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(uv);
    glVertexAttribPointer(
        uv,
        2,
        GL_FLOAT,
        GL_FALSE,
        4*sizeof(GLfloat),
        (GLvoid const*) (2*sizeof(float))
    );
    glEnableVertexAttribArray(uv);
    glUniform4f(rect, geometry.leftOffset, geometry.topOffset, geometry.width, geometry.height);
    glUniform1i(colorMap, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureid);
    glUniformMatrix3fv(linemvp, 1, GL_FALSE, mvp);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableVertexAttribArray(pos);
}
}
