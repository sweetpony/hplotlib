#ifndef COLOR_HPP
#define COLOR_HPP

namespace hpl
{
    struct Color {
        Color() : r(1.0), g(1.0), b(1.0) {}
        Color(float r, float g, float b) : r(r), g(g), b(b) {}
        Color(unsigned int r, unsigned int g, unsigned int b) : r(r/255.0f), g(g/255.0f), b(b/255.0f) {}
        Color(unsigned int hex) : r(((hex >> 16) & 0xff) / 255.0f), g(((hex >> 8) & 0xff) / 255.0f), b((hex & 0xff) / 255.0f) {}
        
        static Color fromHSV(float H, float S, float V);

        float r, g, b;
    };
}

#endif // COLOR_HPP
