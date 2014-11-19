#ifndef OGLPLOTTER_HPP
#define OGLPLOTTER_HPP

#include "AbstractPlotter.hpp"
#include "Window.hpp"
#include "ProgramDatabase.hpp"
#include <iostream>

namespace hpl {
class OGLPlotter : public AbstractPlotter, public Window
{

public:
    OGLPlotter();
    virtual ~OGLPlotter();

    virtual inline void update() {
        checkAndCleanCollections();
        Window::update();
    }

protected:
    struct LineCollection {
        GLuint lineBuffer;
        GLint pos, rect, color, linemvp;
    };
    struct PointCollection {
        GLuint pointBuffer;
        GLint pos, rect, color, pointmvp;
    };
    struct ContourCollection {
        GLuint textureid, mapBuffer;
        GLint pos, uv, rect, colorMap, contourmvp;
    };

    virtual void init();
    virtual void destroy();
    virtual void draw();
    virtual void moveEvent(int deltax, int deltay);
    virtual void mouseWheelEvent(int x, int y, double delta);
    virtual void resetEvent();

    void checkAndCleanCollections();

    ProgramDatabase programsDatabase;

    float mvp[9] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };

    std::map<Drawable::ID, LineCollection> lineCollection;
    std::map<Drawable::ID, PointCollection> pointCollection;
    std::map<Drawable::ID, ContourCollection> contourCollection;

};
}

#endif // OGLPLOTTER_HPP
