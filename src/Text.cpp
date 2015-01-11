#include "Text.hpp"
#include <cstring>

namespace hpl {

Text::Text(std::string const& text, double x, double y, double width, double height, const Limits& limits) :
    Drawable(Drawable::Type_Texture, limits), text(text), x(x), y(y), width(width), height(height)
{
}

Text::~Text()
{
}

double Text::calculateXScale(FontLoader& fontLoader) const
{
    FontTexture* ft = fontLoader.getFont(getFontName());

    float textWidth = 0.0f;
    for (auto it = text.cbegin(); it != text.cend(); ++it) {
        textWidth += ft->ch(*it).xadvance;
    }

    return width / textWidth;
}

double Text::calculateYScale(FontLoader& fontLoader) const
{
    FontTexture* ft = fontLoader.getFont(getFontName());

    float textHeight = ft->header().lineHeight;

    return height / textHeight;
}

bool Text::xScaleDominated(FontLoader& fontLoader) const
{
    return calculateXScale(fontLoader) < calculateYScale(fontLoader);
}

}
