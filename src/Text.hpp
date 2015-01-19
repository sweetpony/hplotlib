#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

#include "Drawable.hpp"
#include "FontLoader.hpp"

namespace hpl
{
struct SimpleText {
    SimpleText(double x, double y, double width, double height) : _x(x), _y(y), _width(width), _height(height) {}
    virtual ~SimpleText() { }

    const double _x, _y, _width, _height;
};

class Text : public Drawable, private SimpleText
{

public:
    Text(std::string const& text, double x, double y, double width, double height, const Limits& limits);
    virtual ~Text();

    virtual inline double x() const {
        return stext->_x;
    }
    virtual inline double y() const {
        return stext->_y;
    }
    virtual inline double width() const {
        return stext->_width;
    }
    virtual inline double height() const {
        return stext->_height;
    }

    virtual inline void setColor(const Color& c) {
        color = c;
        changed.invoke(plotId);
    }
    virtual inline Color getColor() const {
        return color;
    }

    virtual inline void setFont(std::string fontname) {
        this->fontname = fontname;
        changed.invoke(plotId);
    }
    virtual inline const std::string& getFontName() const {
        return fontname;
    }

    virtual inline void setAngle(float angle) {
        this->angle = angle;
        changed.invoke(plotId);
    }
    virtual inline float getAngle() const {
        return angle;
    }

    inline virtual void recalculateData();

    double calculateXScale(FontLoader& fontLoader) const;
    double calculateYScale(FontLoader& fontLoader) const;
    bool xScaleDominated(FontLoader& fontLoader) const;

    std::string text;

protected:
    SimpleText* stext;

    Color color = Color(0.0f, 0.0f, 0.0f);
    std::string fontname = "inconsolata";
    float angle = 0.0f;

};
}

#endif // TEXT_HPP
