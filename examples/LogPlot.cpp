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
        y[i] = pow(x[i], 2.0);
    }
    hpl::Canvas canvas;

    hpl::OGLPlotter plotter;
    canvas.connectToPlotter(&plotter);

    hpl::GridLayout& layout = canvas.addLayout<hpl::GridLayout>();

    hpl::CoordinateSystem& cs = canvas.addCoordinateSystem();
    cs.setColor(hpl::Color(0.0f, 0.0f, 1.0f));
    cs.setAxisProperties(hpl::AxisFlags::PaintPrimary | hpl::AxisFlags::PaintSecondary | hpl::AxisFlags::PaintMinorTicks |
                         hpl::AxisFlags::Logscale | hpl::AxisFlags::PaintLabelsPrimary | hpl::AxisFlags::PaintLabelsSecondary);
    //! @todo adjust
    //cs.getXAxis().setTickMode(hpl::AxisFlags::Smart);
    /*cs.getYAxis().setTickMode(hpl::AxisFlags::Fixed);
    cs.getYAxis().setLimits(-0.3, 1.3);
    cs.getYAxis().setMajorTicks({0.0, 0.25, 0.5, 0.75, 1.0, 1.25});
    cs.getYAxis().setMinorTicks({0.125, 0.375, 0.615, 0.875, 1.125});*/

    hpl::Lines& plot = cs.addPlot<hpl::Lines>(NUM_POINTS, x, y, true);
    plot.setColor(hpl::Color(1.0f, 0.0f, 0.0f));
    plot.setThickness(5.0);

    canvas.addCoordinateSystemToLayout(cs.id(), layout.id());

    hpl::PostscriptPrinter ps(hpl::PlotPrinter::Portrait);
    canvas.connectToPlotter(&ps);
    ps.saveToFile("logplot");

    canvas.synchronise();
    plotter.wait();

    return 0;
}



