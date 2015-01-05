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

    hpl::GridLayout& layout = canvas.addLayout<hpl::GridLayout>();

    hpl::CoordinateSystem& cs1 = canvas.addCoordinateSystem();
    cs1.setColor(hpl::Color(0.6f, 0.6f, 0.6f));
    cs1.setAxisProperties(hpl::AxisFlags::PaintPrimary | hpl::AxisFlags::PaintSecondary | hpl::AxisFlags::PaintMinorTicks | hpl::AxisFlags::PaintLabelsPrimary | hpl::AxisFlags::PaintLabelsSecondary);
    cs1.getXAxis().setMajorTicks({0.0, 0.25, 0.5, 0.75});
    cs1.getXAxis().setMinorTicks({0.125, 0.375, 0.615, 0.875});
    hpl::Lines& plot1 = cs1.addPlot<hpl::Lines>(NUM_POINTS, x, y);
    plot1.setColor(hpl::Color(1.0f, 0.0f, 0.0f));
    plot1.setThickness(5.0);

    hpl::CoordinateSystem& cs2 = canvas.addCoordinateSystem();
    hpl::Contour& plot2 = cs2.addPlot<hpl::Contour>(NUM_POINTS, x, x, m);

    hpl::CoordinateSystem& cs3 = canvas.addCoordinateSystem();
    cs3.setAxisProperties(hpl::AxisFlags::PaintPrimary | hpl::AxisFlags::PaintMinorTicks | hpl::AxisFlags::PaintLabelsPrimary);
    cs3.setTickMode(hpl::AxisFlags::Smart);
    hpl::Points& plot3 = cs3.addPlot<hpl::Points>(NUM_POINTS, x, z);
    plot3.setColor(hpl::Color(0.0f, 0.0f, 1.0f));
    hpl::Lines& plot4 = cs3.addPlot<hpl::Lines>(NUM_POINTS, x, y);
    plot4.setColor(hpl::Color(0.0f, 0.0f, 0.0f));

    canvas.addCoordinateSystemToLayout(cs1.id(), layout.id());
    canvas.addCoordinateSystemToLayout(cs2.id(), layout.id());
    canvas.addCoordinateSystemToLayout(cs3.id(), layout.id());

    hpl::PostscriptPrinter ps(hpl::PlotPrinter::Portrait);
    canvas.connectToPlotter(&ps);
    ps.saveToFile("simplelayout");

    canvas.synchronise();
    plotter.wait();

    return 0;
}

