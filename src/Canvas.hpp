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
#include <queue>

#include "AbstractPlotter.hpp"
#include "Geometry.hpp"
#include "Layout.hpp"
#include "PostscriptPrinter.hpp"
#include "Registry.hpp"

namespace hpl
{
class Canvas {
public:
    Canvas();
	~Canvas();
	
	void addCoordinateSystemToLayout(CoordinateSystem::ID cs, Layout::ID to) { addSlotToLayout(Slot(cs), to); }
	void addLayoutToLayout(Layout::ID layout, Layout::ID to) { addSlotToLayout(Slot(layout), to); }
	
	template<typename T>
	T& addLayout() {
		T* layout = new T;
		Layout::ID id = layouts.add(layout);
		layout->changed.template bind<Canvas, &Canvas::recalculateLayout>(this);
		return *layout;
	}
	
	template<typename T>
    T& addCoordinateSystem();

    //! @todo Canvas should not be the one to instantiate the plotter
    template<typename T>
    T& plotCanvas();
	
private:
	struct Slot {
        Slot(Layout::ID l) : layout(l) {}
		Slot(CoordinateSystem::ID c) : cs(c) {}
		bool operator==(Slot const& other) {
			return layout == other.layout && cs == other.cs;
		}
		void invalidate() {
			layout.invalidate();
			cs.invalidate();
		}
		Layout::ID layout;
		CoordinateSystem::ID cs;
	};
	struct Rack {
		std::vector<Slot> slots;
		std::vector<Geometry> geometries;
	};

	Slot* findSlot(Slot const& slot);
	void addSlotToLayout(Slot const& slot, Layout::ID to);
	void recalculateLayout(Layout::ID layout);

	Registry<Layout> layouts;
	Registry<CoordinateSystem> csystems;
	std::queue<CoordinateSystem::ID> csInit;

    Registry<Drawable> rawData;

    std::unordered_map<Layout::ID, Rack, std::hash<Layout::ID::Type>> racks;
};

template<typename T>
T& Canvas::addCoordinateSystem()
{
    T* cs = new T(rawData);
    //cs->changed.template bind<Window, &Window::update>(this);

    //pthread_mutex_lock(&mutex);
    CoordinateSystem::ID id = csystems.add(cs);
    csInit.push(id);
    //pthread_mutex_unlock(&mutex);

    //update();

    return *cs;
}

template<typename T>
T& Canvas::plotCanvas() {
    T* plotter = new T(rawData);
    return *plotter;
}
}

#endif

