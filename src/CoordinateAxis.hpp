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

    inline void setColor(const Color& c) {
        coordLinesColor = c;
        if (lines != nullptr) {
            lines->setColor(c);
        }
    }
    inline Color getColor() const {
        return coordLinesColor;
    }

    void setLimits(double min, double max);
    void setAxisProperties(int flags);
    void setTickMode(TickMode mode);

private:
    inline bool limitsValid() {
        return min != std::numeric_limits<double>::min() && max != std::numeric_limits<double>::max();
    }

    void setUpCoordLines();
    
    void calculateDataTicks(bool log);
    void calculateSimpleDataPointsForTicks(bool log);
    void calculateSmartDataPointsForTicks(bool log);
    void calculateDataPointsInside(double divisor)t;
    void calculateMinorDataTicks(bool log, double deltaTick);
    
    void setUpAxis(unsigned int indexOffset, double mean);
    void setUpMinorAxis(unsigned int indexOffset, double mean);
    
    void setUpTick(double* primary, double* secondary, double primaryValue, double secondaryMeanValue, double length) const;


    float offset = 0.1f;
    int nrTicks = 8;
    float tickLength = 0.02f;
    int nrMinorTicks = 4;
    float minorTickLength = 0.01f;

    double min = std::numeric_limits<double>::min();
    double max = std::numeric_limits<double>::max();

    std::vector<double> ticks, minorTicks;
    double* rawDataX = nullptr,* rawDataY;
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
    delete lines;
    delete[] rawDataX;
    delete[] rawDataY;
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setLimits(double min, double max)
{
    this->min = min;
    this->max = max;

    setUpCoordLines();

    changed.invoke(Drawable::ID());
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setAxisProperties(int flags) {
    this->flags = flags;
    //! @todo give logscale attribute to plots
    setUpCoordLines();
    changed.invoke(Drawable::ID());
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setTickMode(TickMode mode)
{
    this->tickMode = mode;
    //! @todo do sth with data?
    setUpCoordLines();
    changed.invoke(Drawable::ID());
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setUpCoordLines()
{
    int n = ((flags & Axis_PaintPrimary) ? 1 : 0) + ((flags & Axis_PaintSecondary) ? 1 : 0);
    int l = 0;
    delete[] rawDataX;
    delete[] rawDataY;

    if (n != 0 && limitsValid()) {
        if (flags & (Axis_PaintPrimary | Axis_PaintSecondary)) {
            calculateDataTicks(min, max, flags & Axis_Logscale);
            l += (2 + 2 * ticks.size()) * n;

            //! @todo here and below make this work also for ticks.size() == 1
            if (flags & Axis_PaintMinorTicks && ticks.size() > 1) {
                double delta = (flags & Axis_Logscale ? log10(ticks[1]) - log10(ticks[0]) : ticks[1] - ticks[0]);
                calculateMinorDataTicks(min, max, flags & Axis_Logscale, delta);

                l += 2 * minorTicks.size() * n;
            }
        }

        rawDataX = new double[l];
	rawDataY = new double[l];
        unsigned int o = 0;

        if (flags & Axis_PaintPrimary) {
            setUpAxis(o, offset);
            o += 2 + 2 * ticks.size();

            if (flags & Axis_PaintMinorTicks) {
                setUpAxisMinor(o, offset);
                o += 2 * minorTicks.size();
            }
        }
        if (flags & Axis_PaintSecondary) {
            setUpAxis(o, 1.0);
            o += 2 + 2 * ticks.size();

            if (flags & Axis_PaintMinorTicks) {
                setUpAxisMinor(o, 1.0);
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
        coordLinesID = addNewPlot(coordLines);
        coordLines->setLimits(0.0, 0.0, 1.0, 1.0);
        coordLines->setColor(coordLinesColor);
    } else {
        coordLines = nullptr;
        coordLinesID = Drawable::ID();
    }
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateDataTicks(bool log)
{
    ticks.clear()
    
    switch(tickMode) {
    case FixedAmount:
        calculateSimpleDataPointsForTicks(log);
        break;
    case Smart:
        calculateSmartDataPointsForTicks(log);
        break;
    case Fixed:
        //! @todo implement
        break;
    }
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateSimpleDataPointsForTicks(bool log)
{
    if (log) {
        float spacing = (log10(max) - log10(min)) / (nrTicks + 1);
        for (int i = 0; i < nrTicks; ++i) {
            ticks.push_back(min + pow(10.0, (i+1) * spacing));
        }
    } else {
        float spacing = (max - min) / (nrTicks + 1);
        for (int i = 0; i < nrTicks; ++i) {
            ticks.push_back(min + (i+1) * spacing);
        }
    }
}

template<AxisOrientation orientation>
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

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateDataPointsInside(double divisor)
{
    double val = ceil(min / divisor) * divisor;
    while (val <= max) {
        ticks.push_back(val);
        val += divisor;
    }
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateMinorDataTicks(bool log, double deltaTick)
{
    if (ticks.size() > 0) {
        double delta = deltaTick / (nrMinorTicks + 1);
	double min = this->min;
	double max = this->max;
        if (log) {
            //Assume deltaTick is already in logspace
            min = log10(min);
            max = log10(max);
        }
        double val = ticks[0] - static_cast<unsigned int>((ticks[0] - min) / delta) * delta;
        while (val <= max) {
            if (log) {
                minorTicks.push_back(pow(10.0, val));
            } else {double 
                minorTicks.push_back(val);
            }
            val += delta;
        }
    }
    
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setUpTick(double* primary, double* secondary, double primaryValue, double secondaryMeanValue, double length) const
{
    primary[0] = primaryValue;
    primary[1] = primaryValue;
    secondary[0] = secondaryMeanValue-0.5*length;
    secondary[1] = secondaryMeanValue+0.5*length;
}
}

#endif // COORDINATEAXIS_HPP
