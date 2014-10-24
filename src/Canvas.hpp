/*
 * Canvas.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_CANVAS_HPP
#define HPLOTLIB_CANVAS_HPP

#include <vector>

#include "Window.hpp"
#include "LinePlot.hpp"
#include "Font.hpp"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Layout.hpp"
#include "FixedLayout.hpp"

namespace hpl
{
class Canvas : public Window {
public:
	Canvas(std::string const& fontFile) : fontFile(fontFile) {}
	~Canvas();

    LinePlot* addLinesPlot(int n, double const* x, double const* y, const Geometry& geometry = Geometry());

    inline void setBackgroundColor(const Color& c) {
        backgroundColor = c;
    }

    void setLayout(Layout* layout);

protected:
	virtual void init();
	virtual void destroy();
	virtual void draw();
	virtual void moveEvent(int deltax, int deltay);
	virtual void mouseWheelEvent(int x, int y, double delta);
	virtual void resetEvent();
	
private:
	std::string fontFile;
	Font font;
	int initiated = 0;
    std::vector<Plot*> plots, plots_tmp;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    Color backgroundColor = Color(1.0f, 1.0f, 1.0f);
    Layout* layout = new FixedLayout();
	
	GLuint linevshader = 0;
    GLuint linefshader = 0;
    GLuint lineprogram = 0;
    GLuint textfshader = 0;
    GLuint textprogram = 0;
    
    float mvp[9] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};
};
}

#endif

