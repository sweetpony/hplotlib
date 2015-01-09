#ifndef ABSTRACTPLOTTER_HPP
#define ABSTRACTPLOTTER_HPP

#include <map>
#include "Registry.hpp"
#include "Drawable.hpp"
#include "Lines.hpp"
#include "Points.hpp"
#include "Contour.hpp"
#include "Text.hpp"
#include "Color.hpp"
#include "FontLoader.hpp"

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

    inline void setFontLoader(FontLoader& fontLoader) {
        this->fontLoader = &fontLoader;
    }

    inline void setBackgroundColor(const Color& c) {
        backgroundColor = c;
        update();
    }

    virtual void update() = 0;
    virtual void synchronise() = 0;

protected:    
    const Registry<Drawable>* plots = nullptr;
    std::map<Drawable::ID, unsigned int> revision;
    const std::map<Drawable::ID, unsigned int>* actualRevision = nullptr;
    Color backgroundColor = Color(1.0f, 1.0f, 1.0f);
    FontLoader* fontLoader;
};
}

#endif // ABSTRACTPLOTTER_HPP
