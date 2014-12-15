#ifndef COORDINATEAXIS_HPP
#define COORDINATEAXIS_HPP

#include <cmath>
#include <map>
#include <vector>

#include "Lines.hpp"

namespace hpl {

namespace AxisFlags {
    constexpr int PaintNothing = 0;
    constexpr int PaintPrimary = 1 << 0;
    constexpr int PaintSecondary = 1 << 1;
    constexpr int Logscale = 1 << 2;
    constexpr int PaintMinorTicks = 1 << 3;

    enum AxisOrientation {
        Horizontal,
        Vertical
    };

    enum TickMode {
        FixedAmount,
        Smart,
        Fixed
    };
}

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
    void calculateSimpleDataPointsForTicks(bool log);
    void calculateSmartDataPointsForTicks(bool log);
    void calculateDataPointsInside(double divisor);
    void calculateMinorDataTicks(bool log, double deltaTick);
    
    void setUpAxis(unsigned int indexOffset, double mean);
    void setUpMinorAxis(unsigned int indexOffset, double mean);
    
    void setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length);


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


// Template initialisations

template<AxisFlags::AxisOrientation orientation>
CoordinateAxis<orientation>::CoordinateAxis(Registry<Drawable>& data, std::map<Drawable::ID, unsigned int>& dataRevisions) : data(data), dataRevisions(dataRevisions)
{
    setUpCoordLines();
}

template<AxisFlags::AxisOrientation orientation>
CoordinateAxis<orientation>::~CoordinateAxis()
{
    delete[] rawDataX;
    delete[] rawDataY;
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::setLimits(double xmin, double ymin, double xmax, double ymax)
{
    this->xmin = xmin;
    this->ymin = ymin;
    this->xmax = xmax;
    this->ymax = ymax;

    setUpCoordLines();

    changed.invoke(Drawable::ID());
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::setAxisProperties(int flags) {
    if ((this->flags & AxisFlags::Logscale) != (flags & AxisFlags::Logscale)) {
        changedLogscale.invoke(flags & AxisFlags::Logscale);
    }
    this->flags = flags;
    setUpCoordLines();
    changed.invoke(Drawable::ID());
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::setTickMode(AxisFlags::TickMode mode)
{
    this->tickMode = mode;
    setUpCoordLines();
    changed.invoke(Drawable::ID());
}

template<>
double CoordinateAxis<AxisFlags::Horizontal>::min();
template<>
double CoordinateAxis<AxisFlags::Vertical>::min();
template<>
double CoordinateAxis<AxisFlags::Horizontal>::max();
template<>
double CoordinateAxis<AxisFlags::Vertical>::max();
template<>
float CoordinateAxis<AxisFlags::Horizontal>::offset();
template<>
float CoordinateAxis<AxisFlags::Vertical>::offset();
template<>
float CoordinateAxis<AxisFlags::Horizontal>::otherOffset();
template<>
float CoordinateAxis<AxisFlags::Vertical>::otherOffset();

template<AxisFlags::AxisOrientation orientation>
Drawable::ID CoordinateAxis<orientation>::addNewPlot(Drawable* plot)
{
    Drawable::ID id = data.add(plot);
    plot->setId(id);
    dataRevisions[id] = 1;
    plot->setGeometry(geometry);
    plot->setLimits(xmin, ymin, xmax, ymax);
    plot->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
    changed.invoke(id);
    return id;
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::removePlot(Drawable::ID id)
{
    if (data.has(id)) {
        data.remove(id);
    }

    for (auto it = dataRevisions.begin(); it != dataRevisions.end(); it++) {
        if (it->first == id) {
            dataRevisions.erase(it);
            break;
        }
    }
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::setUpCoordLines()
{
    int n = ((flags & AxisFlags::PaintPrimary) ? 1 : 0) + ((flags & AxisFlags::PaintSecondary) ? 1 : 0);
    int l = 0;
    delete[] rawDataX;
    delete[] rawDataY;

    if (n != 0 && limitsValid()) {
        if (flags & (AxisFlags::PaintPrimary | AxisFlags::PaintSecondary)) {
            calculateDataTicks(flags & AxisFlags::Logscale);
            l += (2 + 2 * ticks.size()) * n;

            //! @todo here and below make this work also for ticks.size() == 1
            if (flags & AxisFlags::PaintMinorTicks && ticks.size() > 1) {
                double delta = (flags & AxisFlags::Logscale ? log10(ticks[1]) - log10(ticks[0]) : ticks[1] - ticks[0]);
                calculateMinorDataTicks(flags & AxisFlags::Logscale, delta);

                l += 2 * minorTicks.size() * n;
            }
        }
        rawDataX = new double[l];
        rawDataY = new double[l];
        unsigned int o = 0;

        if (flags & AxisFlags::PaintPrimary) {
            setUpAxis(o, otherOffset());
            o += 2 + 2 * ticks.size();

            if (flags & AxisFlags::PaintMinorTicks) {
                setUpMinorAxis(o, otherOffset());
                o += 2 * minorTicks.size();
            }
        }
        if (flags & AxisFlags::PaintSecondary) {
            setUpAxis(o, 1.0);
            o += 2 + 2 * ticks.size();

            if (flags & AxisFlags::PaintMinorTicks) {
                setUpMinorAxis(o, 1.0);
                o += 2 * minorTicks.size();
            }
        }
    } else {
        rawDataX = nullptr;
        rawDataY = nullptr;
    }

    if (lines != nullptr) {
        removePlot(linesID);
    }

    if (l != 0) {
        lines = new Lines(l, rawDataX, rawDataY, true);
        linesID = addNewPlot(lines);
        lines->setLimits(0.0, 0.0, 1.0, 1.0);
        lines->setColor(coordLinesColor);
    } else {
        lines = nullptr;
        linesID = Drawable::ID();
    }
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateDataTicks(bool log)
{
    ticks.clear();
    
    switch(tickMode) {
    case AxisFlags::FixedAmount:
        calculateSimpleDataPointsForTicks(log);
        break;
    case AxisFlags::Smart:
        calculateSmartDataPointsForTicks(log);
        break;
    case AxisFlags::Fixed:
        //! @todo implement
        break;
    }
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateSimpleDataPointsForTicks(bool log)
{
    if (log) {
        float spacing = (log10(max()) - log10(min())) / (nrTicks + 1);
        for (int i = 0; i < nrTicks; ++i) {
            ticks.push_back(log10(min()) + (i+1) * spacing);
        }
    } else {
        float spacing = (max() - min()) / (nrTicks + 1);
        for (int i = 0; i < nrTicks; ++i) {
            ticks.push_back(min() + (i+1) * spacing);
        }
    }
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateSmartDataPointsForTicks(bool log)
{
    double div = 10.0, exponent = 1;
    bool todo = true, up = false, down = false;
    unsigned int countMin = 3, countMax = 8;
    int index = 0, size;
    double* steps;

    if (log) {
        size = 1;
        steps = new double[1] {1.0};
    } else {
        size = 3;
        steps = new double[3] {1.0, 2.0, 5.0};
    }

    while (todo) {
        calculateDataPointsInside(div);
        if (ticks.size() > countMax) {
            index ++;
            if (index >= size) {
                index = 0;
                exponent ++;
            }
            div = steps[index] * pow(10.0, exponent);
            down = true;
        } else if (ticks.size() < countMin) {
            index --;
            if (index < 0) {
                index = size - 1;
                exponent --;
            }
            div = steps[index] * pow(10.0, exponent);
            up = true;
            if (down && up) {
                todo = false;
            }
        } else {
            todo = false;
        }
    }

    delete[] steps;
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateDataPointsInside(double divisor)
{
    double val = ceil(min() / divisor) * divisor;
    while (val <= max()) {
        ticks.push_back(val);
        val += divisor;
    }
}

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateMinorDataTicks(bool log, double deltaTick)
{
    minorTicks.clear();

    if (ticks.size() > 0) {
        double delta = deltaTick / (nrMinorTicks + 1);
        double min = this->min();
        double max = this->max();
        if (log) {
            //Assume deltaTick is already in logspace
            min = log10(min);
            max = log10(max);
        }
        double val = ticks[0] - static_cast<unsigned int>((ticks[0] - min) / delta) * delta;
        while (val <= max) {
            if (log) {
                minorTicks.push_back(pow(10.0, val));
            } else {
                minorTicks.push_back(val);
            }
            val += delta;
        }
    }
}

template<>
void CoordinateAxis<AxisFlags::Horizontal>::setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length);
template<>
void CoordinateAxis<AxisFlags::Vertical>::setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length);

template<AxisFlags::AxisOrientation orientation>
void CoordinateAxis<orientation>::setUpMinorAxis(unsigned int indexOffset, double mean)
{
    float spacing = (1.0 - offset()) / (max() - min());
    for (unsigned int i = 0, o = indexOffset+2*i; i < minorTicks.size(); o = indexOffset+2*(++i)) {
        setUpTick(o, offset()+(minorTicks[i]-min())*spacing, mean, minorTickLength);
    }
}
}

#endif // COORDINATEAXIS_HPP
