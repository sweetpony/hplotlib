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
#include <iostream>

#include "Drawable.hpp"
#include "Statistics.hpp"
#include "Contour.hpp"
#include "Registry.hpp"
#include "Lines.hpp"

#include "GL/glld.h"

namespace hpl
{
class CoordinateSystem {
public:
	typedef IDBase<CoordinateSystem> ID;

    struct Label {
        char label[16];
        int len = 0;
        float x, y, width, height;
    };

	static constexpr float XOffset = 0.12f;
	static constexpr float YOffset = 0.08f;
	static constexpr int Ticks = 8;
	static constexpr float TickLength = 0.02f;

    static constexpr int Axis_PaintPrimary = 1 << 0;
    static constexpr int Axis_PaintSecondary = 1 << 1;
    static constexpr int Axis_Logscale = 1 << 2;

    enum TickMode {
        FixedAmount,
        Smart,
        Fixed
    };

    CoordinateSystem(Registry<Drawable>& dataContainer, std::map<Drawable::ID, unsigned int>& dataRevisions);
    ~CoordinateSystem();
    
    void setColor(const Color& c);
    inline Color getColor() const {
        return coordLines->getColor();
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

    void setAxisProperties(int xFlags, int yFlags);
    void setTickMode(TickMode mode);
	
private:
    Drawable::ID addNewPlot(Drawable* plot);
    void removePlot(Drawable::ID id);

    void setUpCoordLines();
    std::vector<double> getDataTicks(double min, double max, bool log) const;
    std::vector<double> getSimpleDataPointsForTicks(double min, double max, bool log) const;
    std::vector<double> getSmartDataPointsForTicks(double min, double max, bool log) const;
    std::vector<double> getDataPointsInside(double min, double max, double divisor) const;
    void setUpHorizontalAxis(double* linesX, double* linesY, unsigned int indexOffset, double yMean, const std::vector<double>& dataTicks) const;
    void setUpVerticalAxis(double* linesX, double* linesY, unsigned int indexOffset, double xMean, const std::vector<double>& dataTicks) const;
    void setUpTick(double* primary, double* secondary, unsigned int indexOffset, double primaryValue, double secondaryMeanValue) const;

    Registry<Drawable>& data;
    std::map<Drawable::ID, unsigned int>& dataRevisions;
    std::vector<Drawable::ID> myPlots;
    ID csId;
	Geometry geometry;

    bool updateLabels = false, needLimitUpdate = true;

	double xmin = std::numeric_limits<double>::min();
	double xmax = std::numeric_limits<double>::max();
	double ymin = std::numeric_limits<double>::min();
	double ymax = std::numeric_limits<double>::max();

    Lines* coordLines = nullptr;
    double* linesX = nullptr,* linesY = nullptr;
    Drawable::ID coordLinesID;
    Color coordLinesColor = Color(0.0f ,0.0f, 0.0f);

    int xFlags = Axis_PaintPrimary, yFlags = Axis_PaintPrimary;
    TickMode tickMode = FixedAmount;

    /*
    GLuint textprogram;
    
    GLint textpos, textuv, textrect, textglyphs, textmvp, textcolor;
	int numChars = 0;

    Label* labels = nullptr;*/
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

