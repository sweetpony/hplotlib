#ifndef COORDINATEAXIS_HPP
#define COORDINATEAXIS_HPP

#include <cmath>
#include <map>
#include <vector>
#include <string>

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

    inline void setGeometry(Geometry geom) {
        geometry = geom;
        setUpCoordLines();
        changed.invoke(Drawable::ID());
    }

    inline float getOffset() {
        return offset();
    }

    void setLimits(double xmin, double xmax, double ymin, double ymax);
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
    float offset();
    float otherOffset();

    void setUpCoordLines();
    
    void calculateDataTicks();
    void calculateSimpleDataPointsForTicks();
    void calculateSmartDataPointsForTicks(bool log);
    void calculateDataPointsInside(double divisor);

    void calculateMinorDataTicks();
    bool canDoNiceLogMinorTicks() const;
    bool ticksAreMagnitudes() const;
    
    void setUpAxis(unsigned int indexOffset, double mean, bool primary);
    void setUpMinorAxis(unsigned int indexOffset, double mean);
    
    void setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length);
    void addLabelToTick(double value, bool primary);

    float xOffset = 0.12f, yOffset = 0.08f;
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

    int flags = AxisFlags::PaintPrimary;
    AxisFlags::TickMode tickMode = AxisFlags::FixedAmount;

    Registry<Drawable>& data;
    std::map<Drawable::ID, unsigned int>& dataRevisions;
    Geometry geometry;
};
}

#include "CoordinateAxis.inl"

#endif // COORDINATEAXIS_HPP
