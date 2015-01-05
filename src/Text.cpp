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

}
