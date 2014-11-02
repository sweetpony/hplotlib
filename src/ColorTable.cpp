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

const ColorTable ColorTable::BlueRed(unsigned int num) {
    ColorTable ct(num);
    ct.interpolateLinear(0.0, 0.0, 1.0, 1.0, 0.0, 0.0);
    return ct;
}

const ColorTable ColorTable::Rainbow() {
    return readColorTableFromFile("../colortables/rainbow.ctbl");
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

void ColorTable::interpolateLinear(float r1, float g1, float b1, float r2, float g2, float b2) {
    for (unsigned int i = 0; i < num; i++) {
        r[i] = r1 + i * (r2 - r1) / num;
        g[i] = g1 + i * (g2 - g1) / num;
        b[i] = b1 + i * (b2 - b1) / num;
    }
}

void ColorTable::interpolateLinearHSV(float h1, float s1, float v1, float h2, float s2, float v2) {
    Color c1 = Color::fromHSV(h1, s1, v1), c2 = Color::fromHSV(h2, s2, v2);
    interpolateLinear(c1.r, c1.g, c1.b, c2.r, c2.g, c2.b);
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
