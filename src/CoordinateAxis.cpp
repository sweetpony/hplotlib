#include "CoordinateAxis.hpp"
#include <cstdio>

namespace hpl {

template<>
void CoordinateAxis<AxisFlags::Horizontal>::setLimits(double min, double max)
{
    originalLimits.setXLimits(min, max);
}

template<>
void CoordinateAxis<AxisFlags::Vertical>::setLimits(double min, double max)
{
    originalLimits.setYLimits(min, max);
}

template<>
double CoordinateAxis<AxisFlags::Horizontal>::min() {
    return limits.xmin();
}

template<>
double CoordinateAxis<AxisFlags::Vertical>::min() {
    return limits.ymin();
}

template<>
double CoordinateAxis<AxisFlags::Horizontal>::max() {
    return limits.xmax();
}

template<>
double CoordinateAxis<AxisFlags::Vertical>::max() {
    return limits.ymax();
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
void CoordinateAxis<AxisFlags::Horizontal>::setUpAxis(unsigned int indexOffset, double mean, bool primary)
{
    rawDataX[indexOffset] = 1.0;
    rawDataY[indexOffset] = mean;
    rawDataX[indexOffset + 1] = offset();
    rawDataY[indexOffset + 1] = mean;

    setUpTicksAndLabels(indexOffset, mean, primary);
}

template<>
void CoordinateAxis<AxisFlags::Vertical>::setUpAxis(unsigned int indexOffset, double mean, bool primary)
{
    rawDataX[indexOffset] = mean;
    rawDataY[indexOffset] = 1.0;
    rawDataX[indexOffset + 1] = mean;
    rawDataY[indexOffset + 1] = offset();

    setUpTicksAndLabels(indexOffset, mean, primary);
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

template<>
void CoordinateAxis<AxisFlags::Horizontal>::addLabelToTick(double pos, double value, bool primary)
{
    float spacing = (1.0 - offset()) / (max() - min());    
    double width = 0.9 * tickdelta * spacing;
    double height = 0.9 * otherOffset();
    double x = pos - 0.5 * width;
    double y = primary ? (0.5 * otherOffset() - 0.5 * height) : (1.0);

    Text* label = new Text(getLabelForTick(value), x, y, width, height, axisLimits);
    addNewLabelToSystem(label);
}

template<>
void CoordinateAxis<AxisFlags::Vertical>::addLabelToTick(double pos, double value, bool primary)
{
	float spacing = (1.0 - offset()) / (max() - min());
	double width = 0.9 * otherOffset();
    double height = 0.9 * tickdelta * spacing;
    double x = 0.5 * otherOffset() - 0.5 * width;
    if (! primary) {
        x += 1.0;
    }
    double y = pos -  0.5 * height;

    Text* label = new Text(getLabelForTick(value), x, y, width, height, axisLimits);
    addNewLabelToSystem(label);
}

}
