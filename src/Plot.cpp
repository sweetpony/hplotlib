#include "Plot.hpp"

namespace hpl
{
Plot::Plot()
{
}

Plot::~Plot()
{
    delete legend;
}


void Plot::init(GLuint lineprogram, GLuint textprogram)
{
    for (auto i = parts.begin(); i != parts.end(); i++) {
        i->second->init(lineprogram, textprogram);
    }
    legend->init(lineprogram, textprogram);
}

void Plot::destroy()
{
    for (auto i = parts.begin(); i != parts.end(); i++) {
        i->second->destroy();
    }
    legend->destroy();
}

void Plot::draw(float const* mvp)
{
    for (auto i = parts.begin(); i != parts.end(); i++) {
        i->second->draw(mvp);
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

void Plot::setColor(PlotPart::ID id, const Color& c)
{
    parts.lookup(id).setColor(c);
    changed.invoke();
}
}
