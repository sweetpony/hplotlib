#include "CoordinateAxis.hpp"

namespace hpl {

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

    float spacing = (1.0 - offset()) / (max() - min());
    bool paintLabels = flags & (primary ? AxisFlags::PaintLabelsPrimary : AxisFlags::PaintLabelsSecondary);

    for (unsigned int i = 0, o = indexOffset+2+2*i; i < ticks.size(); o = indexOffset+2+2*(++i)) {
        setUpTick(o, offset()+(ticks[i]-min())*spacing, mean, tickLength);
        if (paintLabels) {
            addLabelToTick(mean, primary);
        }
    }
}

template<>
void CoordinateAxis<AxisFlags::Vertical>::setUpAxis(unsigned int indexOffset, double mean, bool primary)
{
    rawDataX[indexOffset] = mean;
    rawDataY[indexOffset] = 1.0;
    rawDataX[indexOffset + 1] = mean;
    rawDataY[indexOffset + 1] = offset();

    float spacing = (1.0 - offset()) / (max() - min());
    bool paintLabels = flags & (primary ? AxisFlags::PaintLabelsPrimary : AxisFlags::PaintLabelsSecondary);

    for (unsigned int i = 0, o = indexOffset+2+2*i; i < ticks.size(); o = indexOffset+2+2*(++i)) {
        setUpTick(o, offset()+(ticks[i]-min())*spacing, mean, tickLength);
        if (paintLabels) {
            addLabelToTick(mean, primary);
        }
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

template<>
void CoordinateAxis<AxisFlags::Horizontal>::addLabelToTick(double value, bool primary)
{
    std::string s = std::to_string(value);
    //! @todo these values are not final, y is just a dummy
    double x = value-0.5*tickdelta;
    double y = 0.0;
    //! @todo refactore, see below
    Text* label = new Text(s.length(), s.c_str(), x, y, 0.8*tickdelta, tickLength, axisLimits);
    Drawable::ID labelID = data.add(label);
    label->setId(labelID);
    dataRevisions[labelID] = 1;
    label->setGeometry(geometry);
    label->setColor(coordLinesColor);
    labels.push_back(label);
    labelsIDs.push_back(labelID);
    label->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
}

template<>
void CoordinateAxis<AxisFlags::Vertical>::addLabelToTick(double value, bool primary)
{
    std::string s = std::to_string(value);
    //! @todo these values are not final, y is just a dummy
    double x = value-0.5*tickdelta;
    double y = 0.0;
    //! @todo refactore, see below
    Text* label = new Text(s.length(), s.c_str(), x, y, 0.8*tickdelta, tickLength, axisLimits);
    Drawable::ID labelID = data.add(label);
    label->setId(labelID);
    dataRevisions[labelID] = 1;
    label->setGeometry(geometry);
    label->setColor(coordLinesColor);
    labels.push_back(label);
    labelsIDs.push_back(labelID);
    label->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
}

}
