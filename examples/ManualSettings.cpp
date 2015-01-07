#include <cmath>

#include <Canvas.hpp>
#include <Drawable.hpp>
#include <Color.hpp>
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

    hpl::CoordinateSystem& cs = canvas.addCoordinateSystem();
    cs.setColor(hpl::Color(0.6f, 0.6f, 0.6f));
    cs.setAxisProperties(hpl::AxisFlags::PaintPrimary | hpl::AxisFlags::PaintSecondary | hpl::AxisFlags::PaintMinorTicks | hpl::AxisFlags::PaintLabelsPrimary | hpl::AxisFlags::PaintLabelsSecondary);
    cs.getXAxis().setTickMode(hpl::AxisFlags::Fixed);
    cs.getXAxis().setLimits(-0.3, 1.3);
    cs.getXAxis().setMajorTicks({0.0, 0.25, 0.5, 0.75, 1.0, 1.25});
    cs.getXAxis().setMinorTicks({0.125, 0.375, 0.615, 0.875, 1.125});

    hpl::Lines& plot = cs.addPlot<hpl::Lines>(NUM_POINTS, x, y, true);
    plot.setColor(hpl::Color(1.0f, 0.0f, 0.0f));
    plot.setThickness(5.0);

    canvas.addCoordinateSystemToLayout(cs.id(), layout.id());

    hpl::PostscriptPrinter ps(hpl::PlotPrinter::Portrait);
    canvas.connectToPlotter(&ps);
    ps.saveToFile("manualsettings");

    canvas.synchronise();
    plotter.wait();

    return 0;
}


