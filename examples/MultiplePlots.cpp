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
    double x1[NUM_POINTS];
    double y1[NUM_POINTS];

    for (int i = 0; i < NUM_POINTS; ++i) {
        x1[i] = i / static_cast<double>(NUM_POINTS);
        y1[i] = sin(10.0 * x1[i]);
    }

    double x2[NUM_POINTS];
    double y2[NUM_POINTS];
    double y3[NUM_POINTS];

    for (int i = 0; i < NUM_POINTS; ++i) {
        x2[i] = 2.0 * i / static_cast<double>(NUM_POINTS);
        y2[i] = sin(5.0 * x2[i]);
        y3[i] = cos(5.0 * x2[i]);
    }
    hpl::Canvas canvas;

    hpl::OGLPlotter plotter;
    canvas.connectToPlotter(&plotter);
    plotter.setBackgroundColor(hpl::Color(0.9f, 0.9f, 0.9f));

    hpl::GridLayout& layout = canvas.addLayout<hpl::GridLayout>();
    hpl::CoordinateSystem& cs = canvas.addCoordinateSystem(layout.id());
    cs.setLimitMode(hpl::LimitMode::FixedAfterFirstPlot);

    hpl::Lines& plot1 = cs.addPlot<hpl::Lines>(NUM_POINTS, x1, y1);
    canvas.synchroniseAndSleep(2e6);

    hpl::Lines& plot2 = cs.addPlot<hpl::Lines>(NUM_POINTS, x2, y2);
    plot2.setColor(hpl::Color(0.0f, 0.0f, 1.0f));
    canvas.synchroniseAndSleep(2e6);

    cs.setLimitMode(hpl::LimitMode::RecalculateForEachPlot);
    hpl::Lines& plot3 = cs.addPlot<hpl::Lines>(NUM_POINTS, x2, y3);
    plot3.setColor(hpl::Color(1.0f, 0.0f, 0.0f));

    hpl::PostscriptPrinter ps(hpl::PlotPrinter::Portrait);
    canvas.connectToPlotter(&ps);
    ps.saveToFile("multipleplots");

    canvas.synchroniseAndWait();

    return 0;
}

