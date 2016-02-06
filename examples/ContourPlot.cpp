#include <cmath>

#include <Canvas.hpp>
#include <ColorTable.hpp>

#define NUM_POINTS 100

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

    hpl::CoordinateSystem& cs = canvas.addCoordinateSystem(layout.id());
    cs.setAxisProperties(hpl::Flags<hpl::AxisFlags>(hpl::PaintPrimary, hpl::PaintSecondary, hpl::PaintMinorTicks, hpl::PaintLabelsPrimary));
    hpl::Contour& plot = cs.addPlot<hpl::Contour>(NUM_POINTS, x, x, y);

    hpl::Contour::colorTableRotation.push_back(hpl::ColorTable::getPredefinedTable<hpl::ColorTable::BlueRed>(256));
    hpl::Contour::colorTableRotation.push_back(hpl::ColorTable::getPredefinedTable<hpl::ColorTable::Rainbow>(256));
    hpl::Contour::colorTableRotation.push_back(hpl::ColorTable::getPredefinedTable<hpl::ColorTable::RainbowBlack>(256));
    hpl::Contour::colorTableRotation.push_back(hpl::ColorTable::getPredefinedTable<hpl::ColorTable::RedTemperature>(256));

    hpl::PostscriptPrinter ps(hpl::PlotPrinter::Landscape);
    canvas.connectToPlotter(&ps);
    ps.saveToFile("contourplot");

    canvas.synchroniseAndWait();

    return 0;
}



