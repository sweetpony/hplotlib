/*
 * Canvas.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_CANVAS_HPP
#define HPLOTLIB_CANVAS_HPP

#include "Window.hpp"
#include "ProgramDatabase.hpp"
#include "Plot2D.hpp"
#include "Plot3D.hpp"
#include "Font.hpp"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Layout.hpp"
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
	
	void addPlotToLayout(Plot::ID plot, Layout::ID to) { addSlotToLayout(Slot(plot), to); }
	void addLayoutToLayout(Layout::ID layout, Layout::ID to) { addSlotToLayout(Slot(layout), to); }
	
	template<typename T>
	T& addLayout() {
		T* layout = new T;
		Layout::ID id = layouts.add(layout);
		layout->changed.template bind<Canvas, &Canvas::recalculateLayout>(this);
		return *layout;
	}
	
	template<typename T>
	hpl::Plot& add1D(int n, double const* x, double const* y, Geometry const& geometry = Geometry());

    inline void setBackgroundColor(const Color& c) {
        backgroundColor = c;
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
	struct Slot {
        Slot(Layout::ID l) : layout(l) {}
		Slot(Plot::ID p) : plot(p) {}
		bool operator==(Slot const& other) {
			return layout == other.layout && plot == other.plot;
		}
		void invalidate() {
			layout.invalidate();
			plot.invalidate();
		}
		Layout::ID layout;
		Plot::ID plot;
	};
	struct Rack {
		std::vector<Slot> slots;
		std::vector<Geometry> geometries;
	};

	Slot* findSlot(Slot const& slot);
	void addSlotToLayout(Slot const& slot, Layout::ID to);
	void recalculateLayout(Layout::ID layout);

	Registry<Layout> layouts;
	Registry<Plot> plots;
	std::queue<Plot::ID> plotInit;

	std::unordered_map<Layout::ID, Rack, std::hash<Layout::ID::Type>> racks;

	std::string fontFile;
	Font font;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    Color backgroundColor = Color(1.0f, 1.0f, 1.0f);
	
    ProgramDatabase programsDatabase;
    
    float mvp[9] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};
};

template<typename T>
hpl::Plot& Canvas::add1D(int n, double const* x, double const* y, Geometry const& geometry)
{
    Plot* plot = new Plot;
    plot->changed.template bind<Window, &Window::update>(this);

    Legend* l = new Legend(&font, n, x, y);
    plot->addLegend(l);

    T* p = new T(n, x, y);
    plot->addPlotPart(p);

    pthread_mutex_lock(&mutex);
    Plot::ID id = plots.add(plot);
    plotInit.push(id);
	pthread_mutex_unlock(&mutex);

    update();

    return *plot;
}
}

#endif

