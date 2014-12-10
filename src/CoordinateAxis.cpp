#include "CoordinateAxis.hpp"

namespace hpl {

template<>
CoordinateAxis<Horizontal>::CoordinateAxis(Registry<Drawable>& data, std::map<Drawable::ID, unsigned int>& dataRevisions) :
    offset(0.12f), data(data), dataRevisions(dataRevisions)
{
}

template<>
CoordinateAxis<Vertical>::CoordinateAxis(Registry<Drawable>& data, std::map<Drawable::ID, unsigned int>& dataRevisions) :
    offset(0.08f), data(data), dataRevisions(dataRevisions)
{
}

template<>
double CoordinateAxis<Horizontal>::min() {
    return xmin;
}

template<>
double CoordinateAxis<Vertical>::min() {
    return ymin;
}

template<>
double CoordinateAxis<Horizontal>::max() {
    return xmax;
}

template<>
double CoordinateAxis<Vertical>::max() {
    return ymax;
}

template<>
void CoordinateAxis<Horizontal>::setUpAxis(unsigned int indexOffset, double mean)
{
    rawDataX[indexOffset] = 1.0;
    rawDataY[indexOffset] = mean;
    rawDataX[indexOffset + 1] = offset;
    rawDataY[indexOffset + 1] = mean;

    float spacing = (1.0 - offset) / (max() - min());
    for (unsigned int i = 0, o = indexOffset+2+2*i; i < ticks.size(); ++i, o = indexOffset+2+2*i) {
    setUpTick(o, offset+(ticks[i]-min())*spacing, mean, tickLength);
    }
}

template<>
void CoordinateAxis<Vertical>::setUpAxis(unsigned int indexOffset, double mean)
{
    rawDataX[indexOffset] = mean;
    rawDataY[indexOffset] = 1.0;
    rawDataX[indexOffset + 1] = mean;
    rawDataY[indexOffset + 1] = offset;

    float spacing = (1.0 - offset) / (max() - min());
    for (unsigned int i = 0, o = indexOffset+2+2*i; i < ticks.size(); ++i, o = indexOffset+2+2*i) {
        setUpTick(o, offset+(ticks[i]-min())*spacing, mean, tickLength);
    }
}

template<>
void CoordinateAxis<Horizontal>::setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length)
{
    rawDataX[indexOffset] = primaryValue;
    rawDataX[indexOffset+1] = primaryValue;
    rawDataY[indexOffset] = secondaryMeanValue-0.5*length;
    rawDataY[indexOffset+1] = secondaryMeanValue+0.5*length;
}

template<>
void CoordinateAxis<Vertical>::setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length)
{
    rawDataY[indexOffset] = primaryValue;
    rawDataY[indexOffset+1] = primaryValue;
    rawDataX[indexOffset] = secondaryMeanValue-0.5*length;
    rawDataX[indexOffset+1] = secondaryMeanValue+0.5*length;
}

}
