#include "OGLPlotter.hpp"

namespace hpl {
OGLPlotter::OGLPlotter(const Registry<Drawable>& plots) : AbstractPlotter(plots), Window()
{
    for (auto it = plots.cbegin(); it != plots.cend(); ++it) {
        Lines* l = dynamic_cast<Lines*>(it->second);
        if (l != 0) {
            lineCollection[it->first] = LineCollection();
        }
    }
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

    programsDatabase.init();

    for (auto it = lineCollection.begin(); it != lineCollection.end(); ++it) {
        const Lines* l = static_cast<const Lines*>(&plots.lookup(it->first));

        float* interleave = new float[2 * l->n];
        for (int i = 0; i < l->n; i++) {
            interleave[(i << 1)] = (l->x[i] - l->getXmin()) / (l->getXmax() - l->getXmin());
            interleave[(i << 1) + 1] = (l->y[i] - l->getYmin()) / (l->getYmax() - l->getYmin());
        }

        glGenBuffers(1, &it->second.lineBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, it->second.lineBuffer);
        glBufferData(GL_ARRAY_BUFFER, 2 * l->n * sizeof(float), interleave, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        it->second.pos = glGetAttribLocation(programsDatabase.getLineProgram(), "Position");
        it->second.rect = glGetUniformLocation(programsDatabase.getLineProgram(), "Rect");
        it->second.color = glGetUniformLocation(programsDatabase.getLineProgram(), "Color");
        it->second.linemvp = glGetUniformLocation(programsDatabase.getLineProgram(), "MVP");
    }
}

void OGLPlotter::destroy()
{
    /*pthread_mutex_lock(&mutex);
    for (auto it = csystems.cbegin(); it != csystems.cend(); ++it) {
        it->second->destroy();
    }
    pthread_mutex_unlock(&mutex);*/

    for (auto it = lineCollection.begin(); it != lineCollection.end(); ++it) {
        glDeleteBuffers(1, &it->second.lineBuffer);
    }

    programsDatabase.destroy();
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

    for (auto it = lineCollection.begin(); it != lineCollection.end(); ++it) {
        const Lines* l = static_cast<const Lines*>(&plots.lookup(it->first));
        Geometry g = l->getGeometry();
        Color c = l->getColor();

        glUseProgram(programsDatabase.getLineProgram());
        glBindBuffer(GL_ARRAY_BUFFER, it->second.lineBuffer);
        glVertexAttribPointer(
            it->second.pos,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid const*) 0
        );
        glEnableVertexAttribArray(it->second.pos);
        glUniform4f(it->second.rect, g.leftOffset, g.topOffset, g.width, g.height);
        glUniform3f(it->second.color, c.r, c.g, c.b);
        glUniformMatrix3fv(it->second.linemvp, 1, GL_FALSE, mvp);

        glDrawArrays((l->separate ? GL_LINES : GL_LINE_STRIP), 0, l->n);
        glDisableVertexAttribArray(it->second.pos);
    }
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
