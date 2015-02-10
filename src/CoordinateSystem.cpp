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
    : data(dataContainer), dataRevisions(dataRevisions),
      xAxis(dataContainer, dataRevisions, originalLimits, limits), yAxis(dataContainer, dataRevisions, originalLimits, limits)
{
    xAxis.changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
    xAxis.changedLogscale.template bind<CoordinateSystem, &CoordinateSystem::updateXlogOnPlots>(this);
    yAxis.changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
    yAxis.changedLogscale.template bind<CoordinateSystem, &CoordinateSystem::updateYlogOnPlots>(this);

    limits.changed.template bind<CoordinateAxis<AxisOrientation::Horizontal>, &CoordinateAxis<AxisOrientation::Horizontal>::recalculate>(&xAxis);
    limits.changed.template bind<CoordinateAxis<AxisOrientation::Vertical>, &CoordinateAxis<AxisOrientation::Vertical>::recalculate>(&yAxis);

    originalLimits.changed.template bind<CoordinateSystem, &CoordinateSystem::setLimitsFromOriginal>(this);

    adjustGeometryForPlots(geometry);
}

CoordinateSystem::~CoordinateSystem()
{
}

void CoordinateSystem::setGeometry(Geometry geom)
{
    xAxis.setGeometry(geom);
    yAxis.setGeometry(geom);

    adjustGeometryForPlots(geom);
}

void CoordinateSystem::setMargins(float leftOffset, float rightOffset, float bottomOffset, float topOffset)
{
    Geometry geom(geometry);
    geom.leftOffset = xAxis.getOffset() * geom.width;
    geom.topOffset -= yAxis.getOffset() * geom.height;
    geom.width /= (1.0 - xAxis.getTotalOffset());
    geom.height /= (1.0 - yAxis.getTotalOffset());

    xAxis.setMargins(leftOffset, rightOffset, bottomOffset, topOffset);
    yAxis.setMargins(leftOffset, rightOffset, bottomOffset, topOffset);

    adjustGeometryForPlots(geom);
}

Text& CoordinateSystem::addText(std::string const& text, double x, double y, double width, double height)
{
    Text* plot = new Text(text, x, y, width, height, limits);
    addNewPlot(plot);
    return *plot;
}

void CoordinateSystem::setLimitMode(LimitMode mode)
{
    if (limitMode != mode) {
        limitMode = mode;
    }
}

void CoordinateSystem::setLimits(double xmin, double xmax, double ymin, double ymax)
{
    originalLimits.setLimits(xmin, xmax, ymin, ymax);
    changed.invoke(Drawable::ID());
}

void CoordinateSystem::setLimitsFromAddedPlotIfNeeded(int n, double const* x, double const* y)
{
    if (limitMode == LimitMode::RecalculateForEachPlot || !originalPosLimits.valid()) {
        originalPosLimits.setLimits(minPos(n, x), maxPos(n, x), minPos(n, y), maxPos(n, y));
    }
    if (limitMode == LimitMode::RecalculateForEachPlot || !originalLimits.xValid()) {
        originalLimits.setXLimits(hpl::min(n, x), hpl::max(n, x));
    }
    if (limitMode == LimitMode::RecalculateForEachPlot || !originalLimits.yValid()) {
        originalLimits.setYLimits(hpl::min(n, y), hpl::max(n, y));
    }
}

Drawable::ID CoordinateSystem::addNewPlot(Drawable* plot)
{
    Drawable::ID id = data.add(plot);
    plot->setId(id);
    dataRevisions[id] = 1;
    myPlots.push_back(id);
    plot->setGeometry(geometry);
    plot->setLog(xlog, ylog);
    plot->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
    limits.changed.template bind<Drawable, &Drawable::recalculateData>(plot);
    changed.invoke(id);
    return id;
}

void CoordinateSystem::removePlot(Drawable::ID id)
{
    if (data.has(id)) {
        limits.changed.unbind(&data.lookup(id));
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

void CoordinateSystem::updateXlogOnPlots(bool log)
{
    xlog = log;
    updateLogOnPlots();
}

void CoordinateSystem::updateYlogOnPlots(bool log)
{
    ylog = log;
    updateLogOnPlots();
}

void CoordinateSystem::updateLogOnPlots()
{
    setLimitsFromOriginal();
    for (auto it = myPlots.begin(); it != myPlots.end(); ++it) {
        data.lookup(*it).setLog(xlog, ylog);
    }
}

void CoordinateSystem::setLimitsFromOriginal()
{
    if (xlog && originalPosLimits.xValid()) {
        double mi = originalLimits.xmin(), ma = originalLimits.xmax();
        if (mi <= 0.0) {
            mi = originalPosLimits.xmin();
        }
        if (ma <= 0.0) {
            ma = originalPosLimits.xmax();
        }
        limits.setXLimits(log10(mi), log10(ma));
    } else {
        limits.setXLimits(originalLimits);
    }

    if (ylog && originalPosLimits.yValid()) {
        double mi = originalLimits.ymin(), ma = originalLimits.ymax();
        if (mi <= 0.0) {
            mi = originalPosLimits.ymin();
        }
        if (ma <= 0.0) {
            ma = originalPosLimits.ymax();
        }
        limits.setYLimits(log10(mi), log10(ma));
    } else {
        limits.setYLimits(originalLimits);
    }
}

double* CoordinateSystem::copyData(int n, double const* dat)
{
    double* ret = new double[n];
    std::copy(dat, dat+n, ret);
    return ret;
}

void CoordinateSystem::adjustGeometryForPlots(Geometry geom)
{
    geom.leftOffset += xAxis.getOffset() * geom.width;
    geom.topOffset += yAxis.getOffset() * geom.height;
    geom.width *= (1.0 - xAxis.getTotalOffset());
    geom.height *= (1.0 - yAxis.getTotalOffset());

    geometry = geom;

    for (auto it = myPlots.begin(); it != myPlots.end(); ++it) {
        data.lookup(*it).setGeometry(geom);
    }
}
}
