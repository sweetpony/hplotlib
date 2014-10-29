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
    for (auto it = plots_tmp.cbegin(); it != plots_tmp.cend(); ++it) {
        delete (*it);
    }
    delete layout;
}

void Canvas::setLayout(Layout* layout)
{
    layout->copyPlots(*this->layout);
    delete this->layout;
    this->layout = layout;
    layout->changed.bind<Window, &Window::update>(this);
}

bool Canvas::saveToFile(const std::string& fileName)
{
    PostscriptPrinter p;

    pthread_mutex_lock(&mutex);
    std::vector<Plot*> pl = plots;
    pthread_mutex_unlock(&mutex);

    p.saveToFile(fileName, pl);
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
	for (int i = 0; i < plots.size(); ++i) {
		plots[i]->destroy();
	}
    pthread_mutex_unlock(&mutex);

    programsDatabase.destroy();
}

void Canvas::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	pthread_mutex_lock(&mutex);
	plots_tmp = plots;
	pthread_mutex_unlock(&mutex);

	for (int i = initiated; i < plots_tmp.size(); ++i) {
        plots_tmp[i]->init(programsDatabase.getLineProgram(), programsDatabase.getTextProgram());
	}
	initiated = plots_tmp.size();
	
	for (auto it = plots_tmp.cbegin(); it != plots_tmp.cend(); ++it) {
		(*it)->draw(mvp);
	}
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
