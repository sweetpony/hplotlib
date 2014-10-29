/*
 * Canvas.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "Canvas.hpp"
#include <GL/glld.h>
#include <cmath>
#include <iostream>

namespace hpl
{

Canvas::~Canvas()
{
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
		if (s->plot.valid()) {
			plots.lookup(s->plot).setGeometry(*g);
		} else if (s->layout.valid()) {
			layouts.lookup(s->layout).setGeometry(*g);
			recalculateLayout(s->layout);
		}
	}
	
	update();
}

bool Canvas::saveToFile(const std::string& fileName)
{
    return PostscriptPrinter().saveToFile(fileName, plots);
}
	
void Canvas::init()
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    font.init(fontFile);
    
    programsDatabase.init();
}
	
void Canvas::destroy()
{
	pthread_mutex_lock(&mutex);
	for (auto it = plots.cbegin(); it != plots.cend(); ++it) {
		it->second->destroy();
	}
    pthread_mutex_unlock(&mutex);

    programsDatabase.destroy();
}

void Canvas::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pthread_mutex_lock(&mutex);
	while (!plotInit.empty()) {
		Plot::ID id = plotInit.front();
		plotInit.pop();
		if (plots.has(id)) {
			plots.lookup(id).init(programsDatabase.getLineProgram(), programsDatabase.getTextProgram());
		}
	}
		
	for (auto it = plots.cbegin(); it != plots.cend(); ++it) {
		it->second->draw(mvp);
	}
	pthread_mutex_unlock(&mutex);
}

void Canvas::resetEvent()
{
	mvp[0] = 1.0;
	mvp[4] = 1.0;
	mvp[6] = 0.0;
	mvp[7] = 0.0;
}

void Canvas::moveEvent(int deltax, int deltay)
{
	mvp[6] += deltax / static_cast<double>(width);
	mvp[7] += -deltay / static_cast<double>(height);
}

void Canvas::mouseWheelEvent(int x, int y, double delta)
{
	delta *= 0.2f;
	double old = mvp[0];
	mvp[0] += delta;
	mvp[4] += delta;
	if (mvp[0] < 0.5f) {
		mvp[0] = 0.5f;
	}
	if (mvp[4] < 0.5f) {
		mvp[4] = 0.5f;
	}
	double xs = (x / static_cast<double>(width) - mvp[6]) / old;
	double ys = (1.0 - y / static_cast<double>(height) - mvp[7]) / old;
	mvp[6] += (old-mvp[0]) * xs;
	mvp[7] += (old-mvp[4]) * ys;
}

}
