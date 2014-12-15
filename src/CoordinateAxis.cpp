#include "CoordinateAxis.hpp"

namespace hpl {

template<>
double CoordinateAxis<AxisFlags::Horizontal>::min() {
    return xmin;
}

template<>
double CoordinateAxis<AxisFlags::Vertical>::min() {
    return ymin;
}

template<>
double CoordinateAxis<AxisFlags::Horizontal>::max() {
    return xmax;
}

template<>
double CoordinateAxis<AxisFlags::Vertical>::max() {
    return ymax;
}

template<>
float CoordinateAxis<AxisFlags::Horizontal>::offset() {
    return xOffset;
}

template<>
float CoordinateAxis<AxisFlags::Vertical>::offset() {
    return yOffset;
}

template<>
float CoordinateAxis<AxisFlags::Horizontal>::otherOffset() {
    return yOffset;
}

template<>
float CoordinateAxis<AxisFlags::Vertical>::otherOffset() {
    return xOffset;
}

template<>
void CoordinateAxis<AxisFlags::Horizontal>::setUpAxis(unsigned int indexOffset, double mean)
{
    rawDataX[indexOffset] = 1.0;
    rawDataY[indexOffset] = mean;
    rawDataX[indexOffset + 1] = offset();
    rawDataY[indexOffset + 1] = mean;

    float spacing = (1.0 - offset()) / (max() - min());
    for (unsigned int i = 0, o = indexOffset+2+2*i; i < ticks.size(); o = indexOffset+2+2*(++i)) {
        setUpTick(o, offset()+(ticks[i]-min())*spacing, mean, tickLength);
    }
}

template<>
void CoordinateAxis<AxisFlags::Vertical>::setUpAxis(unsigned int indexOffset, double mean)
{
    rawDataX[indexOffset] = mean;
    rawDataY[indexOffset] = 1.0;
    rawDataX[indexOffset + 1] = mean;
    rawDataY[indexOffset + 1] = offset();

    float spacing = (1.0 - offset()) / (max() - min());
    for (unsigned int i = 0, o = indexOffset+2+2*i; i < ticks.size(); o = indexOffset+2+2*(++i)) {
        setUpTick(o, offset()+(ticks[i]-min())*spacing, mean, tickLength);
    }
}

template<>
void CoordinateAxis<AxisFlags::Horizontal>::setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length)
{
    rawDataX[indexOffset] = primaryValue;
    rawDataX[indexOffset+1] = primaryValue;
    rawDataY[indexOffset] = secondaryMeanValue-0.5*length;
    rawDataY[indexOffset+1] = secondaryMeanValue+0.5*length;
}

template<>
void CoordinateAxis<AxisFlags::Vertical>::setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length)
{
    rawDataY[indexOffset] = primaryValue;
    rawDataY[indexOffset+1] = primaryValue;
    rawDataX[indexOffset] = secondaryMeanValue-0.5*length;
    rawDataX[indexOffset+1] = secondaryMeanValue+0.5*length;
}

}
