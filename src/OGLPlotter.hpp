#ifndef OGLPLOTTER_HPP
#define OGLPLOTTER_HPP

#include "AbstractPlotter.hpp"
#include "Window.hpp"
#include "ProgramDatabase.hpp"
#include "FontTexture.hpp"

namespace hpl {
class OGLPlotter : public AbstractPlotter, public Window
{

public:
    OGLPlotter(std::string const& title = "Canvas");
    virtual ~OGLPlotter();

    virtual inline void update() {
        Window::update();
    }
    virtual inline void synchronise() {
		Window::synchronise();
	}
    virtual inline void wait() {
        Window::wait();
    }

protected:
	virtual void processDrawables();

    struct OGLLines {
        GLuint lineBuffer = 0;
        GLint pos, rect, color, linemvp;
        GLint n;
        Geometry g;
        Color c;
        GLenum type;
        GLfloat size;
    };
    struct OGLPoints {
        GLuint pointBuffer = 0;
        GLint pos, rect, color, pointmvp;
		GLint n;
        Geometry g;
        Color c;
        GLenum type;
        GLfloat size;
    };
    struct OGLContour {
        GLuint textureid = 0, mapBuffer = 0;
        GLint pos, uv, rect, colorMap, contourmvp;
        Geometry g;
    };
    struct OGLText {
		GLuint textBuffer = 0;
		GLint pos, uv, rect, color, glyphs, textmvp;
		GLint n;
		Geometry g;
		Color c;
		FontTexture* font;
	};

    virtual void init();
    virtual void destroy();
    virtual void draw();
    virtual void moveEvent(int deltax, int deltay);
    virtual void mouseWheelEvent(int x, int y, double delta);
    virtual void resetEvent();
    virtual void toggleBackColorTable();
    virtual void toggleColorTable();

    GLenum convert(Drawable::Type type);

    void del(OGLLines& target);
    void del(OGLPoints& target);
    void del(OGLContour& target);
    void del(OGLText& target);

    void syn(Lines const& refthread, OGLLines& target);
    void syn(Points const& ref, OGLPoints& target);
    void syn(Contour const& ref, OGLContour& target);
    void syn(Text const& ref, OGLText& target);

    ProgramDatabase programsDatabase;

    float mvp[9] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };

    std::map<Drawable::ID, OGLLines> lineCollection;
    std::map<Drawable::ID, OGLPoints> pointCollection;
    std::map<Drawable::ID, OGLContour> contourCollection;
    std::map<Drawable::ID, OGLText> textCollection;

    std::vector<std::string> boundTextures;
};
}

#endif // OGLPLOTTER_HPP
