#ifndef COLORBAR_HPP
#define COLORBAR_HPP

namespace hpl
{
struct ColorBar {
    ColorBar(unsigned int num) : num(num), r(new float[num]), g(new float[num]), b(new float[num]) {
    }
    ColorBar(const ColorBar& cb) : num(cb.num), r(new float[num]), g(new float[num]), b(new float[num]) {
        for (unsigned int i = 0; i < num; i++) {
            r[i] = cb.r[i];
            g[i] = cb.g[i];
            b[i] = cb.b[i];
        }
    }

    ~ColorBar() {
        delete[] r;
        delete[] g;
        delete[] b;
    }

    static const ColorBar Rainbow(unsigned int num) {
        ColorBar cb(num);
        cb.interpolateLinear(0.0, 0.0, 1.0, 1.0, 0.0, 0.0);
        return cb;
    }

    ColorBar& operator=(const ColorBar& cb) {
        num = cb.num;
        delete[] r;
        r = new float[num];
        delete[] g;
        g = new float[num];
        delete[] b;
        b = new float[num];
        for (unsigned int i = 0; i < num; i++) {
            r[i] = cb.r[i];
            g[i] = cb.g[i];
            b[i] = cb.b[i];
        }
        return *this;
    }

    void interpolateLinear(float r1, float g1, float b1, float r2, float g2, float b2) {
        for (unsigned int i = 0; i < num; i++) {
            r[i] = r1 + i * (r2 - r1) / num;
            g[i] = g1 + i * (g2 - g1) / num;
            b[i] = b1 + i * (b2 - b1) / num;
        }
    }

    unsigned int num;
    float* r,* g,* b;
};
}

#endif // COLORBAR_HPP
