#include <cmath>

#include <Canvas.hpp>

#define NUM_POINTS 10

int main()
{
    double x[NUM_POINTS];
    double y[NUM_POINTS * NUM_POINTS];

    for (int i = 0; i < NUM_POINTS; ++i) {
        x[i] = i / static_cast<double>(NUM_POINTS);
    }

    for (int i = 0; i < NUM_POINTS; ++i) {
        for (int j = 0; j < NUM_POINTS; ++j) {
            y[i*NUM_POINTS+j] = sin(10.0 * x[i]) * cos(20.0 * x[j]);
        }
    }

    hpl::Canvas canvas;

    hpl::OGLPlotter plotter;
    canvas.connectToPlotter(&plotter);

    hpl::GridLayout& layout = canvas.addLayout<hpl::GridLayout>();

    hpl::CoordinateSystem& cs = canvas.addCoordinateSystem();
    cs.setAxisProperties(hpl::AxisFlags::PaintPrimary | hpl::AxisFlags::PaintSecondary | hpl::AxisFlags::PaintMinorTicks | hpl::AxisFlags::PaintLabelsPrimary);
    hpl::Contour& plot = cs.addPlot<hpl::Contour>(NUM_POINTS, x, x, y);

    canvas.addCoordinateSystemToLayout(cs.id(), layout.id());

    hpl::PostscriptPrinter ps(hpl::PlotPrinter::Landscape);
    canvas.connectToPlotter(&ps);
    ps.saveToFile("contourplot");

	canvas.synchronise();
    plotter.wait();

    return 0;
}



