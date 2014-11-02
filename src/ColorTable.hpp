#ifndef COLORBAR_HPP
#define COLORBAR_HPP

#include <istream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include <iostream>

#include "Color.hpp"

namespace hpl
{
class ColorTable {

public:
    ColorTable(unsigned int num);
    ColorTable(const ColorTable& ct);

    ~ColorTable();

    static const ColorTable BlueRed(unsigned int num);
    static const ColorTable Rainbow();

    ColorTable& operator=(const ColorTable& ct);

    void interpolateLinear(float r1, float g1, float b1, float r2, float g2, float b2);
    void interpolateLinearHSV(float h1, float s1, float v1, float h2, float s2, float v2);

    unsigned int num;
    float* r,* g,* b;

private:
    static const ColorTable readColorTableFromFile(const char* fileName);
};
}

#endif // COLORBAR_HPP
