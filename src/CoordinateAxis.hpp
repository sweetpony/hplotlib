#ifndef COORDINATEAXIS_HPP
#define COORDINATEAXIS_HPP

#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include <iostream>

#include "Lines.hpp"
#include "Text.hpp"
#include "AxisFlags.hpp"
#include "Limits.hpp"

namespace hpl {

template<AxisFlags::AxisOrientation orientation>
class CoordinateAxis
{

public:
    CoordinateAxis(Registry<Drawable>& data, std::map<Drawable::ID, unsigned int>& dataRevisions, Limits& originalLimits, Limits& limits);
    ~CoordinateAxis();

    void setColor(const Color& c);
    inline Color getColor() const {
        return coordLinesColor;
    }

    void setGeometry(Geometry geom);
    void setMargins(float leftOffset, float rightOffset, float bottomOffset, float topOffset);

    inline float getOffset() {
        return primaryParallelOffset();
    }
    inline float getTotalOffset() {
        return totalParallelOffset();
    }

    void setLimits(double xmin, double xmax, double ymin, double ymax);
    void setLimits(double min, double max);
    void setAxisProperties(int flags);
    void setTickMode(AxisFlags::TickMode mode);
    void setMajorTicks(const std::vector<double>& ticks);
    void setMinorTicks(const std::vector<double>& minorTicks);

    void recalculate();

    Delegate<Drawable::ID> changed;
    Delegate<bool> changedLogscale;

private:
    double min();
    double max();
    float primaryParallelOffset();
    float secondaryParallelOffset();
    float primaryPerpendicularOffset();
    float secondaryPerpendicularOffset();
    inline float totalParallelOffset() {
        return primaryParallelOffset() + secondaryParallelOffset();
    }
    inline float totalPerpendicularOffset() {
        return primaryPerpendicularOffset() + secondaryPerpendicularOffset();
    }

    void setUpCoordLines();
    
    void calculateDataTicks();
    void calculateSimpleDataPointsForTicks();
    void calculateSmartDataPointsForTicks(bool log);
    void calculateDataPointsInside(double divisor);
    void setTickDeltaFromTicks();

    void calculateMinorDataTicks();
    bool canDoNiceLogMinorTicks() const;
    bool ticksAreMagnitudes() const;

    void setUpAxis(unsigned int indexOffset, double mean, bool primary);
    void setUpMinorAxis(unsigned int indexOffset, double mean);
    
    void setUpTicksAndLabels(unsigned int indexOffset, double mean, bool primary);

    void setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length);
    void addLabelToTick(double pos, double value, bool primary);
    std::string getLabelForTick(double value);
    void addNewLabelToSystem(Text* label);

    void removeOwnDrawable(Drawable::ID id);

    float leftOffset = 0.12f, rightOffset = 0.12f, bottomOffset = 0.08f, topOffset = 0.08f;
    int nrTicks = 8;
    float tickLength = 0.02f;
    int nrMinorTicks = 4;
    float minorTickLength = 0.01f;

    Limits& originalLimits, & limits, axisLimits;

    std::vector<double> ticks, minorTicks;
    double tickdelta;
    double* rawDataX = nullptr,* rawDataY = nullptr;
    Lines* lines = nullptr;
    std::vector<Text*> labels;
    Drawable::ID linesID;
    std::vector<Drawable::ID> labelsIDs;
    Color coordLinesColor = Color(0.0f ,0.0f, 0.0f);

    int flags = AxisFlags::PaintPrimary | AxisFlags::PaintLabelsPrimary;
    AxisFlags::TickMode tickMode = AxisFlags::FixedAmount;

    Registry<Drawable>& data;
    std::map<Drawable::ID, unsigned int>& dataRevisions;
    Geometry geometry;
};
}

#include "CoordinateAxis.inl"

#endif // COORDINATEAXIS_HPP
