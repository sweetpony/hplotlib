/*
 * Canvas.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_CANVAS_HPP
#define HPLOTLIB_CANVAS_HPP

#include <vector>
#include <string>

#include "Window.hpp"
#include "ProgramDatabase.hpp"
#include "Plot2D.hpp"
#include "Plot3D.hpp"
#include "Font.hpp"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Layout.hpp"
#include "FixedLayout.hpp"
#include "PostscriptPrinter.hpp"

namespace hpl
{
class Canvas : public Window {
public:
	Canvas(std::string const& fontFile) : fontFile(fontFile) {}
	~Canvas();

    template<class PART>
    Plot2D* add2DPlot(int n, double const* x, double const* y, const Geometry& geometry = Geometry());

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
	std::string fontFile;
	Font font;
	int initiated = 0;
    std::vector<Plot*> plots, plots_tmp;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    Color backgroundColor = Color(1.0f, 1.0f, 1.0f);
    Layout* layout = new FixedLayout();
	
    ProgramDatabase programsDatabase;
    
    float mvp[9] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};
};



template<class PART>
Plot2D* Canvas::add2DPlot(int n, double const* x, double const* y, const Geometry& geometry) {
    Plot2D* plot = new Plot2D();
    plot->changed.bind<Window, &Window::update>(this);

    Geometry* lGeo = new Geometry(geometry);
    Legend* l = new Legend(&font, n, x, y, lGeo);
    plot->addLegend(l);

    Geometry* pGeo = new Geometry(geometry);
    PART* p = new PART(n, x, y, pGeo);
    plot->addPlotPart(p);

    layout->addPlot(lGeo, pGeo);

    pthread_mutex_lock(&mutex);
    plots.push_back(plot);
    pthread_mutex_unlock(&mutex);

    needsRepaint = true;

    return plot;
}
}

#endif

