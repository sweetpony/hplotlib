#ifndef ABSTRACTPLOTTER_HPP
#define ABSTRACTPLOTTER_HPP

#include <map>
#include "Registry.hpp"
#include "Drawable.hpp"
#include "Lines.hpp"
#include "Points.hpp"
#include "Color.hpp"

namespace hpl {
class AbstractPlotter
{

public:
    AbstractPlotter() {}
    virtual ~AbstractPlotter() {}

    inline void setPlots(const Registry<Drawable>* plots, const std::map<Drawable::ID, unsigned int>* actualRevision) {
        this->plots = plots;
        this->actualRevision = actualRevision;
    }

    virtual void update() = 0;

protected:
    //virtual void fillBackground() = 0;
    //virtual void drawPoints(/*Points points*/) = 0;
    //virtual void drawLines(/*Lines lines*/) = 0;
    //virtual void drawContour(/*Contour*/) = 0;
    //virtual void drawText(/*Text text*/) = 0;

    const Registry<Drawable>* plots = nullptr;
    std::map<Drawable::ID, unsigned int> revision;
    const std::map<Drawable::ID, unsigned int>* actualRevision;
    Color backgroundColor = Color(1.0f, 1.0f, 1.0f);

};
}

#endif // ABSTRACTPLOTTER_HPP
