/*
 * Canvas.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "Canvas.hpp"
#include "GL/glld.h"
#include <cmath>
#include <chrono>

namespace hpl
{
FontLoader Canvas::fontLoader;

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
}

CoordinateSystem& Canvas::addCoordinateSystem()
{
    CoordinateSystem* cs = new CoordinateSystem(rawData, dataRevisions);
    cs->changed.template bind<Canvas, &Canvas::processUpdate>(this);

    csystems.add(cs);

    processUpdate();

    return *cs;
}

Canvas::Slot* Canvas::findSlot(Slot const& slot)
{
	for (auto it = racks.begin(); it != racks.end(); ++it) {
		auto f = std::find(it->second.slots.begin(), it->second.slots.end(), slot);
		if (f != it->second.slots.end()) {
			return &(*f);
		}
	}
	
	return nullptr;
}

void Canvas::addSlotToLayout(Slot const& slot, Layout::ID to)
{
	Slot* last = findSlot(slot);
	if (last != nullptr) {
		last->invalidate();
	}
	Rack& rack = racks[to];
	rack.slots.push_back(slot);
	rack.geometries.push_back(Geometry());
	
	recalculateLayout(to);
}

void Canvas::recalculateLayout(Layout::ID layout)
{
	Rack& rack = racks[layout];
	layouts.lookup(layout).get(rack.geometries);
	
	auto s = rack.slots.cbegin();
	auto g = rack.geometries.cbegin();
	for (; s != rack.slots.cend() && g != rack.geometries.cend(); ++s, ++g) {
		if (s->cs.valid()) {
			csystems.lookup(s->cs).setGeometry(*g);
		} else if (s->layout.valid()) {
			layouts.lookup(s->layout).setGeometry(*g);
			recalculateLayout(s->layout);
		}
	}

    processUpdate();
}

void Canvas::processUpdate(Drawable::ID id) {
    if (id != Drawable::ID()) {
        ++ dataRevisions[id];
    }
}

CoordinateSystem& Canvas::setUpEasySystem(AbstractPlotter* plotter)
{
    connectToPlotter(plotter);
    GridLayout& layout = addLayout<GridLayout>();
    CoordinateSystem& cosy = addCoordinateSystem();
    addCoordinateSystemToLayout(cosy.id(), layout.id());
    return cosy;
}

void Canvas::synchronise()
{
	for (auto it = connectedPlotters.begin(); it != connectedPlotters.end(); it++) {
        (*it)->synchronise();
        (*it)->update();
    }
}

void Canvas::synchroniseAndSleep(unsigned int useconds)
{
    auto s =  std::chrono::system_clock::now();
    synchronise();
    auto e =  std::chrono::system_clock::now();
    unsigned int usec = std::chrono::duration_cast<std::chrono::microseconds>(e-s).count();
    if (usec <  useconds) {
        sleep(useconds - usec);
    }
}

void Canvas::synchroniseAndWait()
{
    synchronise();
    for (auto it = connectedPlotters.begin(); it != connectedPlotters.end(); it++) {
        (*it)->wait();
    }
}
}
