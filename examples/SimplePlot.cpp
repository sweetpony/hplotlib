#include <cmath>

#include <Canvas.hpp>

#define NUM_POINTS 200

int main()
{
    double x[NUM_POINTS];
    double y[NUM_POINTS];

    for (int i = 0; i < NUM_POINTS; ++i) {
        x[i] = i / static_cast<double>(NUM_POINTS);
        y[i] = sin(10.0 * x[i]);
    }

    hpl::Canvas::plotAndWait<hpl::Lines>(NUM_POINTS, x, y);

    return 0;
}


