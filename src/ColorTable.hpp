#ifndef COLORBAR_HPP
#define COLORBAR_HPP

#include <istream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "Color.hpp"

namespace hpl
{
class ColorTable {

public:
    enum Tables {
        BlueRed,
        Rainbow,
        RainbowBlack,
        RedTemperature
    };

    ColorTable(unsigned int num);
    ColorTable(const ColorTable& ct);

    ~ColorTable();

    template<Tables t>
    static const ColorTable getPredefinedTable(unsigned int num);

    ColorTable& operator=(const ColorTable& ct);

    unsigned int num;
    float* r,* g,* b;

private:
    void interpolateLinear(unsigned int component, float v1, float v2, unsigned int offest = 0, unsigned int length = 0);
    void interpolateLinear(float r1, float g1, float b1, float r2, float g2, float b2, unsigned int offest = 0, unsigned int length = 0);
    void interpolateLinearHSV(float h1, float s1, float v1, float h2, float s2, float v2, unsigned int offest = 0, unsigned int length = 0);

    static const ColorTable readColorTableFromFile(const char* fileName);
};
}

#endif // COLORBAR_HPP
