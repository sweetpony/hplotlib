#include "CoordinateAxis.hpp" //Just for IDE to know the class definitions

namespace hpl {

template<AxisOrientation orientation>
CoordinateAxis<orientation>::CoordinateAxis(Registry<Drawable>& data, std::map<Drawable::ID, unsigned int>& dataRevisions, Limits& originalLimits, Limits& limits) :
    originalLimits(originalLimits), limits(limits), axisLimits(0.0, 1.0, 0.0, 1.0), data(data), dataRevisions(dataRevisions)
{
    setUpCoordLines();
}

template<AxisOrientation orientation>
CoordinateAxis<orientation>::~CoordinateAxis()
{
    delete[] rawDataX;
    delete[] rawDataY;
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setColor(const Color& c)
{
    coordLinesColor = c;
    if (lines != nullptr) {
        lines->setColor(c);
    }
    for (auto it = labels.begin(); it != labels.end(); ++it) {
        (*it)->setColor(c);
    }
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setGeometry(Geometry geom) {
    geometry = geom;
    setUpCoordLines();
    changed.invoke(Drawable::ID());
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setMargins(float leftOffset, float rightOffset, float bottomOffset, float topOffset)

{
    this->leftOffset = leftOffset;
    this->rightOffset = rightOffset;
    this->bottomOffset = bottomOffset;
    this->topOffset = topOffset;

    setUpCoordLines();
    changed.invoke(Drawable::ID());
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setLimits(double xmin, double xmax, double ymin, double ymax)
{
    originalLimits.setLimits(xmin, xmax, ymin, ymax);
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setAxisProperties(Flags<AxisFlags> flags) {
    if ((this->flags & Logscale) != (flags & Logscale)) {
        this->flags = flags;
        changedLogscale.invoke(flags & Logscale);
    } else {
        this->flags = flags;
        setUpCoordLines(); // Only here since logscale change invokes limit update and coord line update from there
    }
    changed.invoke(Drawable::ID());
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setTickMode(TickMode mode)
{
    this->tickMode = mode;
    recalculate();
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setMajorTicks(const std::vector<double>& ticks)
{
    this->ticks = ticks;
    tickMode = TickMode::Fixed;
    recalculate();
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setMinorTicks(const std::vector<double>& minorTicks)
{
    this->minorTicks = minorTicks;
    flags = flags | PaintMinorTicks;
    tickMode = TickMode::Fixed;
    recalculate();
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::recalculate()
{
    setUpCoordLines();
    changed.invoke(Drawable::ID());
}

template<>
double CoordinateAxis<AxisOrientation::Horizontal>::min();
template<>
double CoordinateAxis<AxisOrientation::Vertical>::min();
template<>
double CoordinateAxis<AxisOrientation::Horizontal>::max();
template<>
double CoordinateAxis<AxisOrientation::Vertical>::max();
template<>
float CoordinateAxis<AxisOrientation::Horizontal>::primaryParallelOffset();
template<>
float CoordinateAxis<AxisOrientation::Vertical>::primaryParallelOffset();
template<>
float CoordinateAxis<AxisOrientation::Horizontal>::secondaryParallelOffset();
template<>
float CoordinateAxis<AxisOrientation::Vertical>::secondaryParallelOffset();
template<>
float CoordinateAxis<AxisOrientation::Horizontal>::primaryPerpendicularOffset();
template<>
float CoordinateAxis<AxisOrientation::Vertical>::primaryPerpendicularOffset();
template<>
float CoordinateAxis<AxisOrientation::Horizontal>::secondaryPerpendicularOffset();
template<>
float CoordinateAxis<AxisOrientation::Vertical>::secondaryPerpendicularOffset();

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setUpCoordLines()
{
    int n = ((flags & PaintPrimary) ? 1 : 0) + ((flags & PaintSecondary) ? 1 : 0);
    int l = 0;
    delete[] rawDataX;
    delete[] rawDataY;

    for (unsigned int i = 0; i < labels.size(); ++i) {
        removeOwnDrawable(labelsIDs[i]);
    }
    labels.clear();
    labelsIDs.clear();

    if (n != 0 && limits.valid()) {
        if (flags & (PaintPrimary | PaintSecondary)) {
            calculateDataTicks();
            l += (2 + 2 * ticks.size()) * n;

            if (flags & PaintMinorTicks && ticks.size() > 1) {
                calculateMinorDataTicks();
                l += 2 * minorTicks.size() * n;
            }
        }
        rawDataX = new double[l];
        rawDataY = new double[l];
        unsigned int o = 0;

        if (flags & PaintPrimary) {
            setUpAxis(o, primaryPerpendicularOffset(), true);
            o += 2 + 2 * ticks.size();

            if (flags & PaintMinorTicks) {
                setUpMinorAxis(o, primaryPerpendicularOffset());
                o += 2 * minorTicks.size();
            }
        }
        if (flags & PaintSecondary) {
            setUpAxis(o, 1.0 - secondaryPerpendicularOffset(), false);
            o += 2 + 2 * ticks.size();

            if (flags & PaintMinorTicks) {
                setUpMinorAxis(o, 1.0 - secondaryPerpendicularOffset());
                o += 2 * minorTicks.size();
            }
        }
    } else {
        rawDataX = nullptr;
        rawDataY = nullptr;
    }

    if (lines != nullptr) {
        removeOwnDrawable(linesID);
    }

    if (l != 0) {
        lines = new Lines(l, rawDataX, rawDataY, axisLimits, true);
        linesID = data.add(lines);
        lines->setId(linesID);
        dataRevisions[linesID] = 0;
        lines->setGeometry(geometry);
        lines->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
        changed.invoke(linesID);
        lines->setColor(coordLinesColor);
    } else {
        lines = nullptr;
        linesID = Drawable::ID();
    }
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateDataTicks()
{

    switch(tickMode) {
    case TickMode::FixedAmount:
        ticks.clear();
        calculateSimpleDataPointsForTicks();
        break;
    case TickMode::Smart:
        ticks.clear();
        calculateSmartDataPointsForTicks(flags & Logscale);
        break;
    case TickMode::Fixed:
        setTickDeltaFromTicks();
        break;
    }
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateSimpleDataPointsForTicks()
{
    tickdelta = (max() - min()) / (nrTicks + 1);
    for (int i = 0; i < nrTicks; ++i) {
        ticks.push_back(min() + (i+1) * tickdelta);
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
        steps = new double[1];
		steps[0] = 1.0;
    } else {
        size = 3;
        steps = new double[3];
		steps[0] = 1.0;
		steps[1] = 2.0;
		steps[2] = 5.0;
    }

    while (todo) {
        tickdelta = div;
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
    ticks.clear();
    double val = ceil(min() / divisor) * divisor;
    while (val <= max()) {
        ticks.push_back(val);
        val += divisor;
    }
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setTickDeltaFromTicks()
{
    if (ticks.size() > 1) {
        tickdelta = ticks[1] - ticks[0];
    } else {
        tickdelta = max() - min();
    }
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::calculateMinorDataTicks()
{
    if (tickMode != TickMode::Fixed) {
        minorTicks.clear();

        if (ticks.size() > 0) {
            if (canDoNiceLogMinorTicks()) {
                //Before first tick
                double thismin = (*ticks.begin()) - 1;
                unsigned int mintick = ceil(pow(10.0, min()+fabs(thismin)));
                for (unsigned int i = mintick; i < 10; ++i) {
                    minorTicks.push_back(thismin + log10(i));
                }
                //Between ticks
                for (auto it = ticks.begin(); it != ticks.end(); it++) {
                    for (unsigned int i = 1; i < 10; ++i) {
                        minorTicks.push_back(*it + log10(i));
                    }
                }
                //After last tick
                unsigned int maxtick = ceil(pow(10.0, max()));
                double thismax = *ticks.rbegin();
                for (unsigned int i = 1; i < maxtick; ++i) {
                    minorTicks.push_back(thismax + log10(i));
                }
            } else {
                double delta = tickdelta / (nrMinorTicks + 1);
                double val = ticks[0] - static_cast<unsigned int>((ticks[0] - min()) / delta) * delta;
                while (val <= max()) {
                    minorTicks.push_back(val);
                    val += delta;
                }
            }
        }
    }
}

template<AxisOrientation orientation>
bool CoordinateAxis<orientation>::canDoNiceLogMinorTicks() const
{
    return ((flags & Logscale) && ((tickMode == TickMode::Smart) || ticksAreMagnitudes()));
}

template<AxisOrientation orientation>
bool CoordinateAxis<orientation>::ticksAreMagnitudes() const
{
    bool ret = true, start = true;
    const double epsilon = 0.001;
    double k;
    for (auto it = ticks.begin(); it != ticks.end(); ++it) {
        double l = log10(*it);
        ret &= ((l - static_cast<int>(l)) < epsilon) && (start || ((l-k-1.0) < epsilon));
        k = l;
        start = false;
    }
    return ret;
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setUpTicksAndLabels(unsigned int indexOffset, double mean, bool primary)
{
    float spacing = (1.0 - totalParallelOffset()) / (max() - min());
    bool paintLabels = flags & (primary ? PaintLabelsPrimary : PaintLabelsSecondary);

    for (unsigned int i = 0, o = indexOffset+2+2*i; i < ticks.size(); o = indexOffset+2+2*(++i)) {
        double pos = primaryParallelOffset()+(ticks[i]-min())*spacing;
        setUpTick(o, pos, mean, tickLength);
        if (paintLabels) {
            addLabelToTick(pos, ticks[i], primary);
        }
    }
}

template<>
void CoordinateAxis<AxisOrientation::Horizontal>::setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length);
template<>
void CoordinateAxis<AxisOrientation::Vertical>::setUpTick(unsigned int indexOffset, double primaryValue, double secondaryMeanValue, double length);

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::setUpMinorAxis(unsigned int indexOffset, double mean)
{
    float spacing = (1.0 - totalParallelOffset()) / (max() - min());
    for (unsigned int i = 0, o = indexOffset+2*i; i < minorTicks.size(); o = indexOffset+2*(++i)) {
        setUpTick(o, primaryParallelOffset()+(minorTicks[i]-min())*spacing, mean, minorTickLength);
    }
}
template<>
void CoordinateAxis<AxisOrientation::Horizontal>::addLabelToTick(double pos, double value, bool primary);

template<>
void CoordinateAxis<AxisOrientation::Vertical>::addLabelToTick(double pos, double value, bool primary);

template<AxisOrientation orientation>
std::string CoordinateAxis<orientation>::getLabelForTick(double value)
{
    std::stringstream label;
    label << std::scientific << std::setprecision(2);
    if (flags & Logscale) {
        label << pow(10.0, value);
    } else {
        label << value;
    }
    return label.str();
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::addNewLabelToSystem(Text* label)
{
    Drawable::ID labelID = data.add(label);
    label->setId(labelID);
    dataRevisions[labelID] = 0;
    label->setGeometry(geometry);
    label->setColor(coordLinesColor);
    labels.push_back(label);
    labelsIDs.push_back(labelID);
    label->changed.template bind<Delegate<Drawable::ID>, &Delegate<Drawable::ID>::invoke>(&changed);
}

template<AxisOrientation orientation>
void CoordinateAxis<orientation>::removeOwnDrawable(Drawable::ID id)
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

}
