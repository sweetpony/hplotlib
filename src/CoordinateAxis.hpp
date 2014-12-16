#ifndef COORDINATEAXIS_HPP
#define COORDINATEAXIS_HPP

#include <cmath>
#include <map>
#include <vector>

#include "Lines.hpp"
#include "AxisFlags.hpp"

namespace hpl {

template<AxisFlags::AxisOrientation orientation>
class CoordinateAxis
{

public:
    Delegate<Drawable::ID> changed;
    Delegate<bool> changedLogscale;

    CoordinateAxis(Registry<Drawable>& data, std::map<Drawable::ID, unsigned int>& dataRevisions);
    ~CoordinateAxis();

    inline void setColor(const Color& c) {
        coordLinesColor = c;
        if (lines != nullptr) {
            lines->setColor(c);
        }
    }
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

    void setLimits(double xmin, double ymin, double xmax, double ymax);
    void setAxisProperties(int flags);
    void setTickMode(AxisFlags::TickMode mode);

private:
    double min();
    double max();
    float offset();
    float otherOffset();

    inline bool limitsValid() {
        return xmin != std::numeric_limits<double>::min() && xmax != std::numeric_limits<double>::max()
            && ymin != std::numeric_limits<double>::min() && ymax != std::numeric_limits<double>::max();
    }

    Drawable::ID addNewPlot(Drawable* plot);
    void removePlot(Drawable::ID id);

    void setUpCoordLines();
    
    void calculateDataTicks(bool log);
    void calculateSimpleDataPointsForTicks();
    void calculateSmartDataPointsForTicks(bool log);
    void calculateDataPointsInside(double divisor);
    void calculateMinorDataTicks();
    
    void setUpAxis(unsigned int indexOffset, double mean);
    void setUpMinorAxis(unsigned int indexOffset, double mean);
    
    void setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length);


    //! @todo tickLength should not be fixed but set geometry dependent
    float xOffset = 0.12f, yOffset = 0.08f;
    int nrTicks = 8;
    float tickLength = 0.02f;
    int nrMinorTicks = 4;
    float minorTickLength = 0.01f;

    double xmin = std::numeric_limits<double>::min();
    double xmax = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::min();
    double ymax = std::numeric_limits<double>::max();

    std::vector<double> ticks, minorTicks;
    double tickdelta;
    double* rawDataX = nullptr,* rawDataY = nullptr;
    Lines* lines = nullptr;
    Drawable::ID linesID;
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
