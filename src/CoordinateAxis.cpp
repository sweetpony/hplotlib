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
void CoordinateSystem<Horizontal>::setUpAxis(unsigned int indexOffset, double mean)
{
    rawDataX[indexOffset] = 1.0;
    rawDataY[indexOffset] = mean;
    rawDataX[indexOffset + 1] = offset;
    rawDataY[indexOffset + 1] = mean;

    float spacing = (1.0 - offset) / (max - min);
    for (unsigned int i = 0, o = indexOffset+2+2*i; i < ticks.size(); ++i, o = indexOffset+2+2*i) {
	setUpTick(rawDataX+o, rawDataY+o, offset+(ticks[i]-min)*spacing, mean, tickLength);
    }
}

template<>
void CoordinateSystem<Vertical>::setUpAxis(unsigned int indexOffset, double mean)
{
    rawDataX[indexOffset] = mean;
    rawDataY[indexOffset] = 1.0;
    rawDataX[indexOffset + 1] = mean;
    rawDataY[indexOffset + 1] = offset;

    float spacing = (1.0 - offset) / (max - min);
    for (unsigned int i = 0, o = indexOffset+2+2*i; i < ticks.size(); ++i, o = indexOffset+2+2*i) {
        setUpTick(rawDataY+o, rawDaataX+o, offset+(ticks[i]-min)*spacing, mean, tickLength);
    }
}

template<>
void CoordinateSystem<Horizontal>::setUpMinorAxis(unsigned int indexOffset, double mean)
{
    float spacing = (1.0 - offset) / (max - min);
    for (unsigned int i = 0, o = indexOffset+2*i; i < ticks.size(); ++i, o = indexOffset+2*i) {
        setUpTick(rawDataX + o, rawDataY + o, offset+(ticks[i]-min)*spacing, mean, minorTickLength);
    }
}

template<>
void CoordinateSystem<Vertical>::setUpMinorAxis(unsigned int indexOffset, double mean)
{
    float spacing = (1.0 - offset) / (max - min);
    for (unsigned int i = 0, o = indexOffset+2*i; i < ticks.size(); ++i, o = indexOffset+2*i) {
        setUpTick(rawDataY + o, rawDataX + o, offset+(ticks[i]-min)*spacing, mean, minorTickLength);
    }
}

}
