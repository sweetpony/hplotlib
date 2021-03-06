/*
 * lines.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */

#include <cmath>

#include <Canvas.hpp>
#include <Drawable.hpp>
#include <Color.hpp>
#include <Geometry.hpp>
#include <HorizontalLayout.hpp>
#include <VerticalLayout.hpp>
#include <GridLayout.hpp>
#include <Contour.hpp>
#include <ColorTable.hpp>
#include <OGLPlotter.hpp>

#define NUM_POINTS 200

int main()
{
    double x[NUM_POINTS];
    double y[NUM_POINTS];
    double z[NUM_POINTS];
    double m[NUM_POINTS * NUM_POINTS];
    double n[NUM_POINTS];

    for (int i = 0; i < NUM_POINTS; ++i) {
        x[i] = i / static_cast<double>(NUM_POINTS);
        y[i] = sin(10.0 * x[i]);
        z[i] = cos(20.0 * x[i]);
        n[i] = pow(x[i], 2.0);
    }
    for (int i = 0; i < NUM_POINTS; ++i) {
        for (int j = 0; j < NUM_POINTS; ++j) {
            m[i*NUM_POINTS+j] = sin(10.0 * x[i]) * cos(20.0 * x[j]);
        }
    }
    hpl::Canvas canvas;

    hpl::OGLPlotter plotter;
    canvas.connectToPlotter(&plotter);
    plotter.setBackgroundColor(hpl::Color(0.9f, 0.9f, 0.9f));

    hpl::HorizontalLayout& layout1 = canvas.addLayout<hpl::HorizontalLayout>();
    hpl::VerticalLayout& layout2 = canvas.addLayout<hpl::VerticalLayout>();
    canvas.addLayoutToLayout(layout2.id(), layout1.id());

    hpl::CoordinateSystem& cs1 = canvas.addCoordinateSystem(layout1.id());
    cs1.setColor(hpl::Color(0.6f, 0.6f, 0.6f));
    cs1.setAxisProperties(hpl::Flags<hpl::AxisFlags>(hpl::PaintPrimary, hpl::PaintSecondary, hpl::PaintMinorTicks,
                                                                hpl::PaintLabelsPrimary, hpl::PaintLabelsSecondary));
    cs1.getXAxis().setTickMode(hpl::TickMode::Fixed);
    cs1.getXAxis().setMajorTicks({0.0, 0.25, 0.5, 0.75, 1.0});
    cs1.getXAxis().setMinorTicks({0.125, 0.375, 0.615, 0.875});
    hpl::Lines& plot1 = cs1.addPlot<hpl::Lines>(NUM_POINTS, x, y);
    plot1.setColor(hpl::Color(1.0f, 0.0f, 0.0f));
    plot1.setThickness(5.0);

    hpl::CoordinateSystem& cs2 = canvas.addCoordinateSystem(layout2.id());
    cs2.setAxisProperties(hpl::PaintNothing);
    hpl::Contour& plot2 = cs2.addPlot<hpl::Contour>(NUM_POINTS, x, x, m);

    hpl::CoordinateSystem& cs3 = canvas.addCoordinateSystem(layout2.id());
    cs3.setAxisProperties(hpl::Flags<hpl::AxisFlags>(hpl::PaintPrimary, hpl::Logscale, hpl::PaintMinorTicks, hpl::PaintLabelsPrimary));
    cs3.setTickMode(hpl::TickMode::Smart);
    hpl::Points& plot3 = cs3.addPlot<hpl::Points>(NUM_POINTS, x, z);
    plot3.setColor(hpl::Color(0.0f, 0.0f, 1.0f));
    hpl::Lines& plot4 = cs3.addPlot<hpl::Lines>(NUM_POINTS, x, y);
    plot4.setColor(hpl::Color(0.0f, 0.0f, 0.0f));

    canvas.synchroniseAndSleep(1e6);

    plot2.setColorTable<hpl::ColorTable::BlueRed>(256);
    plot3.setSymbol(hpl::Points::FilledCircle);
    plot3.setSymbolSize(2.0);

    canvas.synchroniseAndSleep(1e6);

    layout2.changeOrientation(hpl::VerticalLayout::BottomToTop);

    //! @todo Interpolation (Dotted) with log and negative values not working
    cs3.getYAxis().setAxisProperties(hpl::Flags<hpl::AxisFlags>(hpl::PaintPrimary, hpl::PaintMinorTicks, hpl::PaintLabelsPrimary));
    plot1.setStyle(hpl::Lines::Dashed);
    plot4.setStyle(hpl::Lines::Dotted);
    plot4.setThickness(3.0);

    canvas.synchroniseAndSleep(1e6);

    layout1.changeOrientation(hpl::HorizontalLayout::RightToLeft);
    plot2.setColorTable<hpl::ColorTable::RainbowBlack>(256);

    hpl::CoordinateSystem& cs4 = canvas.addCoordinateSystem();
    cs4.setTickMode(hpl::TickMode::Smart);
    cs4.setAxisProperties(hpl::Flags<hpl::AxisFlags>(hpl::PaintPrimary, hpl::Logscale, hpl::PaintMinorTicks, hpl::PaintLabelsPrimary));
    hpl::Lines& plot5 = cs4.addPlot<hpl::Lines>(NUM_POINTS, x, n);
    canvas.addCoordinateSystemToLayout(cs4.id(), layout1.id());

    hpl::PostscriptPrinter ps(hpl::PlotPrinter::Portrait);
    canvas.connectToPlotter(&ps);
    ps.saveToFile("combinedplots");

    canvas.synchroniseAndWait();

    return 0;
}

