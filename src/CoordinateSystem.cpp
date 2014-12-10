/*
 * Lines.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "CoordinateSystem.hpp"
#include <cstdio>
#include <algorithm>
#include "Statistics.hpp"

namespace hpl
{
CoordinateSystem::CoordinateSystem(Registry<Drawable>& dataContainer, std::map<Drawable::ID, unsigned int>& dataRevisions)
    : data(dataContainer), dataRevisions(dataRevisions), xAxis(dataContainer, dataRevisions), yAxis(dataContainer, dataRevisions)
{
}

CoordinateSystem::~CoordinateSystem()
{
}

void CoordinateSystem::setGeometry(Geometry geom)
{
    geometry = geom;

    xAxis.setGeometry(geom);
    yAxis.setGeometry(geom);

    geom.leftOffset += xAxis.getOffset() * geom.width;
    geom.topOffset += yAxis.getOffset() * geom.height;
    geom.width *= (1.0 - xAxis.getOffset());
    geom.height *= (1.0 - yAxis.getOffset());

    for (auto it = myPlots.begin(); it != myPlots.end(); ++it) {
        data.lookup(*it).setGeometry(geom);
    }
}

void CoordinateSystem::updateLimits(double xmin, double xmax, double ymin, double ymax)
{
    this->xmin = std::min(this->xmin, xmin);
    this->xmax = std::min(this->xmax, xmax);
    this->ymin = std::min(this->ymin, ymin);
    this->ymax = std::min(this->ymax, ymax);

    xAxis.setLimits(xmin, ymin, xmax, ymax);
    yAxis.setLimits(xmin, ymin, xmax, ymax);

    for (auto it = myPlots.begin(); it != myPlots.end(); ++it) {
        data.lookup(*it).setLimits(this->xmin, this->ymin, this->xmax, this->ymax);
    }

    needLimitUpdate = false;
    changed.invoke(Drawable::ID());
}

Drawable::ID CoordinateSystem::addNewPlot(Drawable* plot)
{
    Drawable::ID id = data.add(plot);
    plot->setId(id);
    dataRevisions[id] = 1;
    myPlots.push_back(id);
    plot->setGeometry(geometry);
    plot->setLimits(xmin, ymin, xmax, ymax);
    plot->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
    changed.invoke(id);
    return id;
}

void CoordinateSystem::removePlot(Drawable::ID id)
{
    if (data.has(id)) {
        data.remove(id);
    }

    for (auto it = myPlots.begin(); it != myPlots.end(); it++) {
        if (*it == id) {
            myPlots.erase(it);
            break;
        }
    }

    for (auto it = dataRevisions.begin(); it != dataRevisions.end(); it++) {
        if (it->first == id) {
            dataRevisions.erase(it);
            break;
        }
    }
}
}
