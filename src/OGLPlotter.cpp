#include "OGLPlotter.hpp"

namespace hpl {
OGLPlotter::OGLPlotter() : AbstractPlotter(), Window()
{
}

OGLPlotter::~OGLPlotter()
{
}

void OGLPlotter::init()
{
    if (plots != 0) {
        for (auto it = plots->cbegin(); it != plots->cend(); ++it) {
            if (revision.find(it->first) == revision.end()) {
                Lines* l = dynamic_cast<Lines*>(it->second);
                if (l != 0) {
                    lineCollection[it->first] = LineCollection();
                } else {
                    Points* p = dynamic_cast<Points*>(it->second);
                    if (p != 0) {
                        pointCollection[it->first] = PointCollection();
                    } else {
                        Contour* c = dynamic_cast<Contour*>(it->second);
                        if (c != 0) {
                            contourCollection[it->first] = ContourCollection();
                        }
                    }
                }
                revision[it->first] = 0;
            }
        }
    }

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //font.init(fontFile);

    programsDatabase.init();

    for (auto it = lineCollection.begin(); it != lineCollection.end(); ++it) {
        unsigned int ar = actualRevision->at(it->first);
        if (ar != revision[it->first]) {
            const Lines* l = static_cast<const Lines*>(&plots->lookup(it->first));

            float* interleave = new float[2 * l->n];
            for (int i = 0; i < l->n; i++) {
                interleave[(i << 1)] = (l->x[i] - l->getXmin()) / (l->getXmax() - l->getXmin());
                interleave[(i << 1) + 1] = (l->y[i] - l->getYmin()) / (l->getYmax() - l->getYmin());
            }

            if (revision[it->first] != 0) {
                glDeleteBuffers(1, &it->second.lineBuffer);
            }
            glGenBuffers(1, &it->second.lineBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, it->second.lineBuffer);
            glBufferData(GL_ARRAY_BUFFER, 2 * l->n * sizeof(float), interleave, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            it->second.pos = glGetAttribLocation(programsDatabase.getLineProgram(), "Position");
            it->second.rect = glGetUniformLocation(programsDatabase.getLineProgram(), "Rect");
            it->second.color = glGetUniformLocation(programsDatabase.getLineProgram(), "Color");
            it->second.linemvp = glGetUniformLocation(programsDatabase.getLineProgram(), "MVP");

            delete[] interleave;
            revision[it->first] = ar;
        }
    }

    for (auto it = pointCollection.begin(); it != pointCollection.end(); ++it) {
        unsigned int ar = actualRevision->at(it->first);
        if (ar != revision[it->first]) {
            const Points* p = static_cast<const Points*>(&plots->lookup(it->first));

            float* interleave = new float[2 * p->n];
            for (int i = 0; i < p->n; i++) {
                interleave[(i << 1)] = (p->x[i] - p->getXmin()) / (p->getXmax() - p->getXmin());
                interleave[(i << 1) + 1] = (p->y[i] - p->getYmin()) / (p->getYmax() - p->getYmin());
            }

            if (revision[it->first] != 0) {
                glDeleteBuffers(1, &it->second.pointBuffer);
            }
            glGenBuffers(1, &it->second.pointBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, it->second.pointBuffer);
            glBufferData(GL_ARRAY_BUFFER, 2 * p->n * sizeof(float), interleave, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            it->second.pos = glGetAttribLocation(programsDatabase.getLineProgram(), "Position");
            it->second.rect = glGetUniformLocation(programsDatabase.getLineProgram(), "Rect");
            it->second.color = glGetUniformLocation(programsDatabase.getLineProgram(), "Color");
            it->second.pointmvp = glGetUniformLocation(programsDatabase.getLineProgram(), "MVP");

            delete[] interleave;
            revision[it->first] = ar;
        }
    }

    for (auto it = contourCollection.begin(); it != contourCollection.end(); ++it) {
        unsigned int ar = actualRevision->at(it->first);
        if (ar != revision[it->first]) {
            const Contour* c = static_cast<const Contour*>(&plots->lookup(it->first));
            const ColorTable& colorTable = c->getColorTable();

            float rectCorners[16];
            rectCorners[0] = 0.0f; rectCorners[1] = 1.0f;
            rectCorners[2] = 0.0f; rectCorners[3] = 1.0f;
            rectCorners[4] = 0.0f; rectCorners[5] = 0.0f;
            rectCorners[6] = 0.0f; rectCorners[7] = 0.0f;
            rectCorners[8] = 1.0f; rectCorners[9] = 0.0f;
            rectCorners[10] = 1.0f; rectCorners[11] = 0.0f;
            rectCorners[12] = 1.0f; rectCorners[13] = 1.0f;
            rectCorners[14] = 1.0f; rectCorners[15] = 1.0f;

            if (revision[it->first] != 0) {
                glDeleteBuffers(1, &it->second.mapBuffer);
            }
            glGenBuffers(1, &it->second.mapBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, it->second.mapBuffer);
            glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), rectCorners, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            it->second.pos = glGetAttribLocation(programsDatabase.getMapProgram(), "Position");
            it->second.uv = glGetAttribLocation(programsDatabase.getMapProgram(), "UV");
            it->second.rect = glGetUniformLocation(programsDatabase.getMapProgram(), "Rect");
            it->second.colorMap = glGetUniformLocation(programsDatabase.getMapProgram(), "ColorMap");
            it->second.contourmvp = glGetUniformLocation(programsDatabase.getMapProgram(), "MVP");

            if (revision[it->first] != 0) {
                glDeleteTextures(1, &it->second.textureid);
            }
            glGenTextures(1, &it->second.textureid);

            glBindTexture(GL_TEXTURE_2D, it->second.textureid);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            float* data = new float[3 * c->n * c->n];
            for(int i = 0; i < c->n*c->n; ++i) {
                double zi = (c->z[i] - c->getZmin()) / (c->getZmax() - c->getZmin());
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

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, c->n, c->n, 0, GL_RGB, GL_FLOAT, data);

            delete[] data;
            revision[it->first] = ar;
        }
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

    for (auto it = pointCollection.begin(); it != pointCollection.end(); ++it) {
        glDeleteBuffers(1, &it->second.pointBuffer);
    }

    for (auto it = contourCollection.begin(); it != contourCollection.end(); ++it) {
        glDeleteBuffers(1, &it->second.mapBuffer);
        glDeleteTextures(1, &it->second.textureid);
    }

    programsDatabase.destroy();
}

void OGLPlotter::draw()
{
    init();

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
        const Lines* l = static_cast<const Lines*>(&plots->lookup(it->first));
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

    for (auto it = pointCollection.begin(); it != pointCollection.end(); ++it) {
        const Points* p = static_cast<const Points*>(&plots->lookup(it->first));
        Geometry g = p->getGeometry();
        Color c = p->getColor();

        glUseProgram(programsDatabase.getLineProgram());
        glBindBuffer(GL_ARRAY_BUFFER, it->second.pointBuffer);
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
        glUniformMatrix3fv(it->second.pointmvp, 1, GL_FALSE, mvp);

        glDrawArrays(GL_POINTS, 0, p->n);
        glDisableVertexAttribArray(it->second.pos);
    }

    for (auto it = contourCollection.begin(); it != contourCollection.end(); ++it) {
        const Contour* c = static_cast<const Contour*>(&plots->lookup(it->first));
        Geometry g = c->getGeometry();

        glUseProgram(programsDatabase.getMapProgram());
        glBindBuffer(GL_ARRAY_BUFFER, it->second.mapBuffer);
        glVertexAttribPointer(
            it->second.pos,
            2,
            GL_FLOAT,
            GL_FALSE,
            4*sizeof(GLfloat),
            (GLvoid const*) 0
        );
        glEnableVertexAttribArray(it->second.pos);
        glEnableVertexAttribArray(it->second.uv);
        glVertexAttribPointer(
            it->second.uv,
            2,
            GL_FLOAT,
            GL_FALSE,
            4*sizeof(GLfloat),
            (GLvoid const*) (2*sizeof(float))
        );
        glUniform4f(it->second.rect, g.leftOffset, g.topOffset, g.width, g.height);
        glUniform1i(it->second.colorMap, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, it->second.textureid);
        glUniformMatrix3fv(it->second.contourmvp, 1, GL_FALSE, mvp);

        glDrawArrays(GL_QUADS, 0, 4);
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

void OGLPlotter::checkAndCleanCollections()
{
    for (auto it = lineCollection.begin(); it != lineCollection.end();) {
        if (!plots->has(it->first)) {
            lineCollection.erase(it++);
        } else {
            ++it;
        }
    }
}
}
