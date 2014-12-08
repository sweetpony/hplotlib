#ifndef COORDINATEAXIS_HPP
#define COORDINATEAXIS_HPP

#include <cmath>
#include <map>
#include <vector>

#include "Lines.hpp"

namespace hpl {
enum AxisOrientation {
    Horizontal,
    Vertical
};

template<AxisOrientation orientation>
class CoordinateAxis
{

public:
    static constexpr int PaintPrimary = 1 << 0;
    static constexpr int PaintSecondary = 1 << 1;
    static constexpr int Logscale = 1 << 2;
    static constexpr int PaintMinorTicks = 1 << 3;

    enum TickMode {
        FixedAmount,
        Smart,
        Fixed
    };

    CoordinateAxis(Registry<Drawable>& data, std::map<Drawable::ID, unsigned int>& dataRevisions);
    ~CoordinateAxis();

private:
    float offset = 0.1f;
    int nrTicks = 8;
    float tickLength = 0.02f;
    int nrMinorTicks = 4;
    float minorTickLength = 0.01f;

    double min = std::numeric_limits<double>::min();
    double max = std::numeric_limits<double>::max();

    std::vector<double> ticks, minorTicks;
    double* rawData = nullptr;
    Lines* lines = nullptr;
    Drawable::ID linesID;
    Color coordLinesColor = Color(0.0f ,0.0f, 0.0f);

    int flags = PaintPrimary;
    TickMode tickMode = FixedAmount;

    Registry<Drawable>& data;
    std::map<Drawable::ID, unsigned int>& dataRevisions;

    Delegate<Drawable::ID> changed;

};


// Template initialisations

template<AxisOrientation orientation>
CoordinateAxis<orientation>::~CoordinateAxis()
{
}
}

#endif // COORDINATEAXIS_HPP
