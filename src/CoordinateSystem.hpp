/*
 * CoordinateSystem.hpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#ifndef HPLOTLIB_COORDINATESYSTEM_HPP
#define HPLOTLIB_COORDINATESYSTEM_HPP

#include <queue>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>

#include "Drawable.hpp"
#include "Statistics.hpp"
#include "Contour.hpp"
#include "Registry.hpp"
#include "Lines.hpp"
#include "CoordinateAxis.hpp"
#include "Limits.hpp"

#include "GL/glld.h"

namespace hpl
{

enum class LimitMode {
    FixedAfterFirstPlot,
    RecalculateForEachPlot
};

class CoordinateSystem {
public:
    typedef IDBase<CoordinateSystem> ID;

    CoordinateSystem(Registry<Drawable>& dataContainer, std::map<Drawable::ID, unsigned int>& dataRevisions);
    ~CoordinateSystem();

    inline CoordinateAxis<AxisOrientation::Horizontal>& getXAxis() {
        return xAxis;
    }
    inline CoordinateAxis<AxisOrientation::Vertical>& getYAxis() {
        return yAxis;
    }

    inline void setColor(const Color& c) {
        xAxis.setColor(c);
        yAxis.setColor(c);
    }
    inline void setAxisProperties(Flags<AxisFlags> flags) {
        xAxis.setAxisProperties(flags);
        yAxis.setAxisProperties(flags);
    }
    inline void setTickMode(TickMode mode) {
        xAxis.setTickMode(mode);
        yAxis.setTickMode(mode);
    }

    void setGeometry(Geometry geom);
    void setMargins(float leftOffset, float rightOffset, float bottomOffset, float topOffset);
    
    template<typename T>
    T& addPlot(int n, double const* x, double const* y, double copyData = false);
    template<typename T>
    T& addPlot(int n, double const* x, double const* y, double const* z, double copyData = false);
    Text& addText(std::string const& text, double x, double y, double width, double height);

    void setLimitMode(LimitMode mode);
    void setLimits(double xmin, double xmax, double ymin, double ymax);

    Delegate<Drawable::ID> changed;
    
    inline ID id() const { return csId; }
    inline void setId(ID id) { csId = id; }
	
private:
    void setLimitsFromAddedPlotIfNeeded(int n, double const* x, double const* y);
    Drawable::ID addNewPlot(Drawable* plot);
    void removePlot(Drawable::ID id);

    void updateXlogOnPlots(bool log);
    void updateYlogOnPlots(bool log);
    void updateLogOnPlots();

    void setLimitsFromOriginal();

    double* copyData(int n, double const* dat);

    void adjustGeometryForPlots(Geometry geom);

    Registry<Drawable>& data;
    std::map<Drawable::ID, unsigned int>& dataRevisions;
    std::vector<Drawable::ID> myPlots;
    ID csId;
	Geometry geometry;
    bool xlog = false, ylog = false;

    LimitMode limitMode = LimitMode::FixedAfterFirstPlot;
    Limits originalLimits, originalPosLimits, limits;

    CoordinateAxis<AxisOrientation::Horizontal> xAxis;
    CoordinateAxis<AxisOrientation::Vertical> yAxis;
};

template<typename T>
T& CoordinateSystem::addPlot(int n, double const* x, double const* y, double copyData)
{
    setLimitsFromAddedPlotIfNeeded(n, x, y);

    if (copyData) {
        x = this->copyData(n, x);
        y = this->copyData(n, y);
    }

    T* plot = new T(n, x, y, limits);
    addNewPlot(plot);
    return *plot;
}

template<typename T>
T& CoordinateSystem::addPlot(int n, double const* x, double const* y, double const* z, double copyData)
{
    setLimitsFromAddedPlotIfNeeded(n, x, y);

    if (copyData) {
        x = this->copyData(n, x);
        y = this->copyData(n, y);
        z = this->copyData(n*n, z);
    }

    T* plot = new T(n, x, y, z, limits);
    addNewPlot(plot);
    return *plot;
}
}

#endif

