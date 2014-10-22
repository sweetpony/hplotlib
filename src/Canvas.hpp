/*
 * Canvas.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef PLOT_CANVAS_HPP
#define PLOT_CANVAS_HPP

#include <vector>

#include "Window.hpp"
#include "Lines.hpp"
#include "Legend.hpp"
#include "Font.hpp"

namespace plot
{
class Canvas : public Window {
public:
	Canvas(std::string const& fontFile) : fontFile(fontFile) {}
	~Canvas();
	void addLinesPlot(Uint n, Doub const* x, Doub const* y, float r, float g, float b, float left, float top, float width, float height);

protected:
	virtual void init();
	virtual void destroy();
	virtual void draw();
	virtual void moveEvent(double deltax, double deltay);
	virtual void mouseWheelEvent(double x, double y, double delta);
	virtual void resetEvent();
	
private:
	std::string fontFile;
	Font font;
	int initiated = 0;
	std::vector<Plot*> plots;
	std::vector<Plot*> plots_tmp;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	
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

