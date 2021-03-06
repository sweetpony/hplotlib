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

    hpl::CoordinateSystem& cs = canvas.addCoordinateSystem(layout.id());
    cs.setColor(hpl::Color(0.0f, 0.0f, 1.0f));
    cs.setAxisProperties(hpl::Flags<hpl::AxisFlags>(hpl::PaintPrimary, hpl::PaintSecondary, hpl::PaintMinorTicks,
                         hpl::Logscale, hpl::PaintLabelsPrimary, hpl::PaintLabelsSecondary));
    //! @todo enable this to see bug
    //cs.getXAxis().setLimits(-2.5, 0.5);
    cs.getXAxis().setTickMode(hpl::TickMode::Smart);
    cs.getYAxis().setTickMode(hpl::TickMode::Fixed);
    cs.getYAxis().setMajorTicks({-4.0, -3.0, -2.0, -1.0, 0.0});

    hpl::Lines& plot = cs.addPlot<hpl::Lines>(NUM_POINTS, x, y, true);
    plot.setColor(hpl::Color(1.0f, 0.0f, 0.0f));
    plot.setThickness(5.0);

    hpl::PostscriptPrinter ps(hpl::PlotPrinter::Portrait);
    canvas.connectToPlotter(&ps);
    ps.saveToFile("logplot");

    canvas.synchroniseAndWait();

    return 0;
}



