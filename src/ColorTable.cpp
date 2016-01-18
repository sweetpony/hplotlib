#include "ColorTable.hpp"

namespace hpl {
ColorTable::ColorTable(unsigned int num) : num(num), r(new float[num]), g(new float[num]), b(new float[num]) {
}

ColorTable::ColorTable(const ColorTable& ct) : num(ct.num), r(new float[num]), g(new float[num]), b(new float[num]) {
    for (unsigned int i = 0; i < num; i++) {
        r[i] = ct.r[i];
        g[i] = ct.g[i];
        b[i] = ct.b[i];
    }
}

ColorTable::~ColorTable() {
    delete[] r;
    delete[] g;
    delete[] b;
}

template<>
const ColorTable ColorTable::getPredefinedTable<ColorTable::Tables::BlueRed>(unsigned int num) {
    ColorTable ct(num);
    ct.interpolateLinear(0.0, 0.0, 1.0, 1.0, 0.0, 0.0);
    return ct;
}

template<>
const ColorTable ColorTable::getPredefinedTable<ColorTable::Tables::Rainbow>(unsigned int num) {
    ColorTable ct(num);
    unsigned int n4 = num / 4;
    ct.interpolateLinear(0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0, n4);
    ct.interpolateLinear(0.0, 1.0, 1.0, 0.0, 1.0, 0.0, n4, n4);
    ct.interpolateLinear(0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 2*n4, n4);
    ct.interpolateLinear(1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 3*n4, num - 3*n4);
    return ct;
}

template<>
const ColorTable ColorTable::getPredefinedTable<ColorTable::Tables::RainbowBlack>(unsigned int) {
    return readColorTableFromFile("../colortables/RainbowBlack.ctbl");
}

template<>
const ColorTable ColorTable::getPredefinedTable<ColorTable::Tables::RedTemperature>(unsigned int num) {
    ColorTable ct(num);
    unsigned int n1 = 165 * (num * 1.0 / 256);
    ct.interpolateLinear(0, 0.0, 1.0, 0, n1);
    ct.interpolateLinear(0, 1.0, 1.0, n1, num - n1);
    unsigned int n2 = 128 * (num * 1.0 / 256);
    ct.interpolateLinear(1, 0.0, 0.0, 0, n2);
    ct.interpolateLinear(1, 0.0, 1.0, n2, num - n2);
    unsigned int n3 = 169 * (num * 1.0 / 256);
    ct.interpolateLinear(2, 0.0, 0.0, 0, n3);
    ct.interpolateLinear(2, 0.0, 1.0, n3, num - n3);
    return ct;
}

ColorTable& ColorTable::operator=(const ColorTable& ct) {
    num = ct.num;
    delete[] r;
    r = new float[num];
    delete[] g;
    g = new float[num];
    delete[] b;
    b = new float[num];

    for (unsigned int i = 0; i < num; i++) {
        r[i] = ct.r[i];
        g[i] = ct.g[i];
        b[i] = ct.b[i];
    }
    return *this;
}


void ColorTable::interpolateLinear(unsigned int component, float v1, float v2, unsigned int offset, unsigned int length)
{
    if (length == 0) {
        length = num;
    }

    float* v;
    switch(component) {
    case 0:
        v = r;
        break;
    case 1:
        v = g;
        break;
    case 2:
        v = b;
        break;
    default:
        break;
    }

    for (unsigned int i = offset, j = 0; j < length; i++, j++) {
        v[i] = v1 + j * (v2 - v1) / length;
    }
}


void ColorTable::interpolateLinear(float r1, float g1, float b1, float r2, float g2, float b2, unsigned int offset, unsigned int length) {
    if (length == 0) {
        length = num;
    }
    for (unsigned int i = offset, j = 0; j < length; i++, j++) {
        r[i] = r1 + j * (r2 - r1) / length;
        g[i] = g1 + j * (g2 - g1) / length;
        b[i] = b1 + j * (b2 - b1) / length;
    }
}

void ColorTable::interpolateLinearHSV(float h1, float s1, float v1, float h2, float s2, float v2, unsigned int offset, unsigned int length) {
    Color c1 = Color::fromHSV(h1, s1, v1), c2 = Color::fromHSV(h2, s2, v2);
    interpolateLinear(c1.r, c1.g, c1.b, c2.r, c2.g, c2.b, offset, length);
}

const ColorTable ColorTable::readColorTableFromFile(const char* fileName)
{
    std::ifstream i;
    i.open(fileName, std::ios_base::in);
    if (i) {
        int N = std::count(std::istreambuf_iterator<char>(i), std::istreambuf_iterator<char>(), '\n');
        i.seekg(0);
        ColorTable ct(N);

        int j = 0;
        for(std::string line; std::getline(i, line) && j < N; j++) {
            std::istringstream in(line);
            in >> ct.r[j] >> ct.g[j] >> ct.b[j];
        }
        i.close();

        return ct;
    } else {
        return ColorTable(1);
    }
}
}
