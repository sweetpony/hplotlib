#ifndef TEXT_HPP
#define TEXT_HPP

#include "Drawable.hpp"
#include "Font.hpp"

namespace hpl
{
class Text : public Drawable
{

public:
    Text(int n, char const* text, double x, double y, double width, double height) : Drawable(Drawable::Type_Texture), n(n), text(text), x(x), y(y), width(width), height(height) {}
    virtual ~Text() {}

    virtual inline void setColor(const Color& c) {
        color = c;
        changed.invoke(plotId);
    }
    virtual inline Color getColor() const {
        return color;
    }

    virtual inline void setFont(std::string path) {
        font.init(path);
        changed.invoke(plotId);
    }
    virtual inline const Font& getFont() {
        return font;
    }

    virtual inline void setAngle(float angle) {
        this->angle = angle;
        changed.invoke(plotId);
    }
    virtual inline float getAngle() const {
        return angle;
    }

    const int n;
    const char* text;
    const double x, y, width, height;

protected:
    Color color = Color(0.0f, 0.0f, 0.0f);
    Font font;
    float angle;

};

#endif // TEXT_HPP
