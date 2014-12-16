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

#include "Drawable.hpp"
#include "Statistics.hpp"
#include "Contour.hpp"
#include "Registry.hpp"
#include "Lines.hpp"
#include "CoordinateAxis.hpp"

#include "GL/glld.h"

namespace hpl
{
class CoordinateSystem {
public:
    typedef IDBase<CoordinateSystem> ID;

    CoordinateSystem(Registry<Drawable>& dataContainer, std::map<Drawable::ID, unsigned int>& dataRevisions);
    ~CoordinateSystem();

    inline CoordinateAxis<AxisFlags::Horizontal>& getXAxis() {
        return xAxis;
    }
    inline CoordinateAxis<AxisFlags::Vertical>& getYAxis() {
        return yAxis;
    }

    inline void setColor(const Color& c) {
        xAxis.setColor(c);
        yAxis.setColor(c);
    }
    inline void setAxisProperties(int flags) {
        xAxis.setAxisProperties(flags);
        yAxis.setAxisProperties(flags);
    }
    inline void setTickMode(AxisFlags::TickMode mode) {
        xAxis.setTickMode(mode);
        yAxis.setTickMode(mode);
    }

    void setGeometry(Geometry geom);
    
    template<typename T>
    T& addPlot(int n, double const* x, double const* y);
    template<typename T>
    T& addPlot(int n, double const* x, double const* y, double const* z);

    void updateLimits(double xmin, double xmax, double ymin, double ymax);

    Delegate<Drawable::ID> changed;
    
    inline ID id() const { return csId; }
    inline void setId(ID id) { csId = id; }
	
private:
    Drawable::ID addNewPlot(Drawable* plot);
    void removePlot(Drawable::ID id);

    inline bool limitsValid() {
        return xmin != std::numeric_limits<double>::min() && xmax != std::numeric_limits<double>::max()
            && ymin != std::numeric_limits<double>::min() && ymax != std::numeric_limits<double>::max();
    }

    void updateXlogOnPlots(bool log);
    void updateYlogOnPlots(bool log);
    void updateLogOnPlots();

    Registry<Drawable>& data;
    std::map<Drawable::ID, unsigned int>& dataRevisions;
    std::vector<Drawable::ID> myPlots;
    ID csId;
	Geometry geometry;
    bool xlog = false, ylog = false;

    bool needLimitUpdate = true;

	double xmin = std::numeric_limits<double>::min();
	double xmax = std::numeric_limits<double>::max();
	double ymin = std::numeric_limits<double>::min();
    double ymax = std::numeric_limits<double>::max();

    CoordinateAxis<AxisFlags::Horizontal> xAxis;
    CoordinateAxis<AxisFlags::Vertical> yAxis;
};

template<typename T>
T& CoordinateSystem::addPlot(int n, double const* x, double const* y)
{
    if (needLimitUpdate) {
        updateLimits(hpl::min(n, x), hpl::max(n, x), hpl::min(n, y), hpl::max(n, y));
    }

    T* plot = new T(n, x, y);
    addNewPlot(plot);
    return *plot;
}

template<typename T>
T& CoordinateSystem::addPlot(int n, double const* x, double const* y, double const* z)
{
    if (needLimitUpdate) {
        updateLimits(hpl::min(n, x), hpl::max(n, x), hpl::min(n, y), hpl::max(n, y));
    }

    T* plot = new T(n, x, y, z);
    addNewPlot(plot);
    return *plot;
}
}

#endif

