#ifndef OGLPLOTTER_HPP
#define OGLPLOTTER_HPP

#include "AbstractPlotter.hpp"
#include "Window.hpp"
#include "ProgramDatabase.hpp"

namespace hpl {
class OGLPlotter : public AbstractPlotter, public Window
{

public:
    OGLPlotter(const Registry<Drawable>& plots);
    virtual ~OGLPlotter();

protected:
    struct LineCollection {
        GLuint lineBuffer;
        GLint pos, rect, color, linemvp;
    };

    virtual void init();
    virtual void destroy();
    virtual void draw();
    virtual void moveEvent(int deltax, int deltay);
    virtual void mouseWheelEvent(int x, int y, double delta);
    virtual void resetEvent();

    ProgramDatabase programsDatabase;

    float mvp[9] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };

    std::map<Drawable::ID, LineCollection> lineCollection;

};
}

#endif // OGLPLOTTER_HPP
