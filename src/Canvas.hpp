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
#include "OGLPlotter.hpp"
#include "Geometry.hpp"
#include "Layout.hpp"
#include "GridLayout.hpp"
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
        layouts.add(layout);
		layout->changed.template bind<Canvas, &Canvas::recalculateLayout>(this);
		return *layout;
	}

    CoordinateSystem& addCoordinateSystem();

    inline void connectToPlotter(AbstractPlotter* plotter) {
        plotter->setPlots(&rawData, &dataRevisions);
        connectedPlotters.push_back(plotter);
    }

    template<typename T>
    static void plotAndWait(int n, double const* x, double const* y) {
        Canvas canvas;
        OGLPlotter plotter;
        CoordinateSystem& cosy = canvas.setUpEasySystem(&plotter);
        cosy.addPlot<T>(n, x, y);
        plotter.wait();
    }

    template<typename T>
    static void plotAndWait(int n, double const* x, double const* y, double const* z) {
        Canvas canvas;
        OGLPlotter plotter;
        CoordinateSystem& cosy = canvas.setUpEasySystem(&plotter);
        cosy.addPlot<T>(n, x, y, z);
        plotter.wait();
    }
    
    void synchronise();

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

    void processUpdate(Drawable::ID id = Drawable::ID());

    CoordinateSystem& setUpEasySystem(AbstractPlotter* plotter);

	Registry<Layout> layouts;
    Registry<CoordinateSystem> csystems;

    Registry<Drawable> rawData;
    std::map<Drawable::ID, unsigned int> dataRevisions;

    std::unordered_map<Layout::ID, Rack, std::hash<Layout::ID::Type>> racks;

    std::vector<AbstractPlotter*> connectedPlotters;
};
}

#endif

