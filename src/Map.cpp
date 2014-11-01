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

    n = 1;
    rectCorners = new float[8];
    rectCorners[0] = 0.78f; rectCorners[1] = 0.78f;
    rectCorners[2] = 0.78f; rectCorners[3] = 0.11f;
    rectCorners[4] = 0.88f; rectCorners[5] = 0.78f;
    rectCorners[6] = 0.88f; rectCorners[7] = 0.11f;
}

Map::~Map()
{
    delete[] rectCorners;
}

void Map::init(GLuint mapprogram, GLuint)
{
    program = mapprogram;

    glGenBuffers(1, &mapBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mapBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), rectCorners, GL_STATIC_DRAW);
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

    unsigned int width = 2, height = 2;
    //! @todo data points here!
    float data[] = {0.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f};

    glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY8, width, height, 0, GL_RGB, GL_FLOAT, data);
}

void Map::destroy()
{
    glDeleteBuffers(1, &mapBuffer);
    glDeleteTextures(1, &textureid);
}

void Map::draw(float const* mvp)
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
