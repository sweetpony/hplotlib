#ifndef ABSTRACTPLOTTER_HPP
#define ABSTRACTPLOTTER_HPP

#include <map>
#include "Registry.hpp"
#include "Drawable.hpp"
#include "Lines.hpp"
#include "Color.hpp"

namespace hpl {
class AbstractPlotter
{

public:
    AbstractPlotter() {}
    virtual ~AbstractPlotter() {}

    inline void setPlots(const Registry<Drawable>* plots) {
        this->plots = plots;
    }

protected:
    //virtual void fillBackground() = 0;
    //virtual void drawPoints(/*Points points*/) = 0;
    //virtual void drawLines(/*Lines lines*/) = 0;
    //virtual void drawContour(/*Contour*/) = 0;
    //virtual void drawText(/*Text text*/) = 0;

    const Registry<Drawable>* plots = nullptr;
    std::map<Drawable::ID, unsigned int> revision;
    Color backgroundColor = Color(1.0f, 1.0f, 1.0f);

};
}

#endif // ABSTRACTPLOTTER_HPP
