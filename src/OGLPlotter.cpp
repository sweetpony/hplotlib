#include "OGLPlotter.hpp"

namespace hpl {
OGLPlotter::OGLPlotter(const Registry<Plot>& plots) : AbstractPlotter(plots), Window()
{
}

OGLPlotter::~OGLPlotter()
{
}

void OGLPlotter::init()
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //font.init(fontFile);

    for (auto it = lineCollection.begin(); it != lineCollection.end(); it++) {
        const Lines* l = static_cast<const Lines*>(&plots.lookup(it->first));

        float* interleave = new float[2 * l->n];
        //! @todo fill interleave

        glGenBuffers(1, &it->second.lineBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, it->second.lineBuffer);
        glBufferData(GL_ARRAY_BUFFER, 2 * l->n * sizeof(float), interleave, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        it->second.pos = glGetAttribLocation(programsDatabase.getLineProgram(), "Position");
        it->second.rect = glGetUniformLocation(programsDatabase.getLineProgram(), "Rect");
        it->second.color = glGetUniformLocation(programsDatabase.getLineProgram(), "Color");
        it->second.linemvp = glGetUniformLocation(programsDatabase.getLineProgram(), "MVP");
    }

    programsDatabase.init();
}

void OGLPlotter::destroy()
{
    /*pthread_mutex_lock(&mutex);
    for (auto it = csystems.cbegin(); it != csystems.cend(); ++it) {
        it->second->destroy();
    }
    pthread_mutex_unlock(&mutex);*/

    programsDatabase.destroy();

    //! @todo for lines
    //glDeleteBuffers(1, &lineBuffer);
}

void OGLPlotter::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*pthread_mutex_lock(&mutex);
    while (!csInit.empty()) {
        CoordinateSystem::ID id = csInit.front();
        csInit.pop();
        if (csystems.has(id)) {
            csystems.lookup(id).init(programsDatabase.getLineProgram(), programsDatabase.getTextProgram(), programsDatabase.getMapProgram());
        }
    }

    for (auto it = csystems.cbegin(); it != csystems.cend(); ++it) {
        it->second->draw(mvp);
    }
    pthread_mutex_unlock(&mutex);*/

    //! @todo for lines
    /*
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

    glDrawArrays(GL_LINE_STRIP, 0, n);
    glDisableVertexAttribArray(pos);*/
}

void OGLPlotter::resetEvent()
{
    mvp[0] = 1.0;
    mvp[4] = 1.0;
    mvp[6] = 0.0;
    mvp[7] = 0.0;
}

void OGLPlotter::moveEvent(int deltax, int deltay)
{
    mvp[6] += deltax / static_cast<double>(width);
    mvp[7] += -deltay / static_cast<double>(height);
}

void OGLPlotter::mouseWheelEvent(int x, int y, double delta)
{
    delta *= 0.2f;
    double old = mvp[0];
    mvp[0] += delta;
    mvp[4] += delta;
    if (mvp[0] < 0.5f) {
        mvp[0] = 0.5f;
    }
    if (mvp[4] < 0.5f) {
        mvp[4] = 0.5f;
    }
    double xs = (x / static_cast<double>(width) - mvp[6]) / old;
    double ys = (1.0 - y / static_cast<double>(height) - mvp[7]) / old;
    mvp[6] += (old-mvp[0]) * xs;
    mvp[7] += (old-mvp[4]) * ys;
}
}
