#ifndef ABSTRACTPLOTTER_HPP
#define ABSTRACTPLOTTER_HPP

#include "PaintServer.hpp"
#include "Color.hpp"

namespace hpl {
class AbstractPlotter
{

public:
    AbstractPlotter(const PaintServer& paintServer);
    virtual ~AbstractPlotter();

protected:
    virtual void fillBackground() = 0;
    virtual void drawPoints(/*Points points*/) = 0;
    virtual void drawLines(/*Lines lines*/) = 0;
    virtual void drawContour(/*Contour*/) = 0;
    virtual void drawText(/*Text text*/) = 0;

    const PaintServer& paintServer;

    Color backgroundColor = Color(1.0f, 1.0f, 1.0f);

};
}

#endif // ABSTRACTPLOTTER_HPP
