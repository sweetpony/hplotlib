#ifndef ABSTRACTPLOTTER_HPP
#define ABSTRACTPLOTTER_HPP

#include <map>
#include "Registry.hpp"
#include "Plot.hpp"
#include "Lines.hpp"
#include "Color.hpp"

namespace hpl {
class AbstractPlotter
{

public:
    AbstractPlotter(const Registry<Plot>& plots);
    virtual ~AbstractPlotter();

protected:
    virtual void fillBackground() = 0;
    virtual void drawPoints(/*Points points*/) = 0;
    virtual void drawLines(/*Lines lines*/) = 0;
    virtual void drawContour(/*Contour*/) = 0;
    virtual void drawText(/*Text text*/) = 0;

    const Registry<Plot>& plots;
    std::map<Plot::ID, unsigned int> revision;
    Color backgroundColor = Color(1.0f, 1.0f, 1.0f);

};
}

#endif // ABSTRACTPLOTTER_HPP
