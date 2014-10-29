#include "Plot.hpp"

namespace hpl
{
Plot::Plot()
{
}

Plot::~Plot()
{
    for (std::vector<PlotPart*>::iterator i = parts.begin(); i != parts.end(); i++) {
        delete *i;
    }
    delete legend;
}


void Plot::init(GLuint lineprogram, GLuint textprogram)
{
    for (std::vector<PlotPart*>::iterator i = parts.begin(); i != parts.end(); i++) {
        (*i)->init(lineprogram, textprogram);
    }
    legend->init(lineprogram, textprogram);
}

void Plot::destroy()
{
    for (std::vector<PlotPart*>::iterator i = parts.begin(); i != parts.end(); i++) {
        (*i)->destroy();
    }
    legend->destroy();
}

void Plot::draw(float const* mvp)
{
    for (std::vector<PlotPart*>::iterator i = parts.begin(); i != parts.end(); i++) {
        (*i)->draw(mvp);
    }
    legend->draw(mvp);
}

void Plot::addLegend(Legend* legend)
{
    this->legend = legend;
    changed.invoke();
}

void Plot::setLegendColor(const Color& c)
{
    legend->setColor(c);
    changed.invoke();
}

PlotPart::ID Plot::addPlotPart(PlotPart* plotPart)
{
    parts.push_back(plotPart);
    changed.invoke();
    return parts.size()-1;
}

void Plot::setColor(PlotPart::ID id, const Color& c)
{
    parts[id]->setColor(c);
    changed.invoke();
}
}
