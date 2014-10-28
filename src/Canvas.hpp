/*
 * Canvas.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_CANVAS_HPP
#define HPLOTLIB_CANVAS_HPP

#include "Window.hpp"
#include "LinePlot.hpp"
#include "ScatterPlot.hpp"
#include "Font.hpp"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Layout.hpp"
#include "FixedLayout.hpp"
#include "PostscriptPrinter.hpp"
#include "Registry.hpp"

#include <vector>
#include <string>
#include <queue>

namespace hpl
{
class Canvas : public Window {
public:
	Canvas(std::string const& fontFile) : fontFile(fontFile) {}
	~Canvas();
	
	template<typename T>
	T& addLayout() {
		T* layout = new T;
		layouts.add(layout);
		return *layout;
	}
	
	template<typename T>
	T& add1D(int n, double const* x, double const* y, Geometry const& geometry = Geometry());
	
	void remove(Plot::ID id) {
		// TODO
	}

    LinePlot* addLinesPlot(int n, double const* x, double const* y, const Geometry& geometry = Geometry());
    ScatterPlot* addScatterPlot(int n, double const* x, double const* y, const Geometry& geometry = Geometry());

    inline void setBackgroundColor(const Color& c) {
        backgroundColor = c;
    }

    void setLayout(Layout* layout);
    inline Layout* getLayout() {
        return layout;
    }

    bool saveToFile(const std::string& fileName);


protected:
	virtual void init();
	virtual void destroy();
	virtual void draw();
	virtual void moveEvent(int deltax, int deltay);
	virtual void mouseWheelEvent(int x, int y, double delta);
	virtual void resetEvent();
	
private:
	Registry<Layout> layouts;
	Registry<Plot> plots;
	std::queue<Plot::ID> plotInit;

	std::string fontFile;
	Font font;
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

template<typename T>
T& Canvas::add1D(int n, double const* x, double const* y, Geometry const& geometry)
{
    T* plot = new T();
    plot->changed.template bind<Window, &Window::update>(this);

    Geometry* lGeo = new Geometry(geometry);
    Legend* l = new Legend(&font, n, x, y, lGeo);
    plot->addLegend(l);

    Geometry* pGeo = new Geometry(geometry);
    Lines* p = new Lines(n, x, y, pGeo);
    plot->addLines(p);

    layout->addPlot(lGeo, pGeo);

    pthread_mutex_lock(&mutex);
    Plot::ID id = plots.add(plot);
    plotInit.push(id);
	pthread_mutex_unlock(&mutex);

    update();

    return *plot;
}
}

#endif

