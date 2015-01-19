#include "Text.hpp"
#include <cstring>
#include <cmath>

namespace hpl {

Text::Text(std::string const& text, double x, double y, double width, double height, const Limits& limits) :
    Drawable(Drawable::Type_Texture, limits), SimpleText(x, y, width, height), text(text), stext(new SimpleText(x, y, width, height))
{
}

Text::~Text()
{
    delete stext;
}

void Text::recalculateData()
{
    delete stext;

    double x = (xlog ? log10(_x) : _x);
    double y = (ylog ? log10(_y) : _y);

    double w = _width;
    double h = _height;
    //! @todo or?
    //double w = (xlog ? log10(_x + _width) - x : _width);
    //double h = (ylog ? log10(_y + _height) - y : _height);

    stext = new SimpleText(x, y, w, h);

    changed.invoke(plotId);
}

double Text::calculateXScale(FontLoader& fontLoader) const
{
    FontTexture* ft = fontLoader.getFont(getFontName());

    float textWidth = 0.0f;
    for (auto it = text.cbegin(); it != text.cend(); ++it) {
        textWidth += ft->ch(*it).xadvance;
    }

    return width() / textWidth;
}

double Text::calculateYScale(FontLoader& fontLoader) const
{
    FontTexture* ft = fontLoader.getFont(getFontName());

    float textHeight = ft->header().lineHeight;

    return height() / textHeight;
}

bool Text::xScaleDominated(FontLoader& fontLoader) const
{
    return calculateXScale(fontLoader) < calculateYScale(fontLoader);
}

}
