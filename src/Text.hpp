#ifndef TEXT_HPP
#define TEXT_HPP

#include "Drawable.hpp"

namespace hpl
{
//! @todo need to redsign slightly to make use of xlog and ylog
class Text : public Drawable
{

public:
    Text(std::string const& text, double x, double y, double width, double height, const Limits& limits);
    virtual ~Text();

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

    inline virtual void recalculateData() {}

    std::string text;
    const double x, y, width, height;

protected:
    Color color = Color(0.0f, 0.0f, 0.0f);
    std::string fontname = "inconsolata";
    float angle = 0.0f;

};
}

#endif // TEXT_HPP
