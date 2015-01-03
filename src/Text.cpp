#include "Text.hpp"

namespace hpl {

Text::Text(int n, char const* text, double x, double y, double width, double height, const Limits& limits) :
    Drawable(Drawable::Type_Texture, limits), n(n), text(text), x(x), y(y), width(width), height(height)
{
    std::cout << "Add text: " << this->text << " from " << text << std::endl;
}
Text::~Text()
{
}

}
