#include <cmath>

#include <Canvas.hpp>
#include <GridLayout.hpp>
#include <OGLPlotter.hpp>

#define NUM_POINTS 200

int main()
{
    double x[NUM_POINTS];
    double y[NUM_POINTS];

    for (int i = 0; i < NUM_POINTS; ++i) {
        x[i] = i / static_cast<double>(NUM_POINTS);
        y[i] = sin(10.0 * x[i]);
    }
    hpl::Canvas canvas;

    hpl::OGLPlotter plotter;
    canvas.connectToPlotter(&plotter);
    plotter.setBackgroundColor(hpl::Color(0.9f, 0.9f, 0.9f));

    hpl::GridLayout& layout = canvas.addLayout<hpl::GridLayout>();

    for (int i = 0; i < 4*4; ++i) {
        hpl::CoordinateSystem& cs = canvas.addCoordinateSystem();
        hpl::Lines& plot = cs.addPlot<hpl::Lines>(NUM_POINTS, x, y, true);

        canvas.addCoordinateSystemToLayout(cs.id(), layout.id());

        canvas.synchronise();
    }

    hpl::PostscriptPrinter ps(hpl::PlotPrinter::Portrait);
    canvas.connectToPlotter(&ps);
    ps.saveToFile("biggrid");

    canvas.synchronise();
    plotter.wait();

    return 0;
}


