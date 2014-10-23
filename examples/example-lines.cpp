/*
 * lines.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */

#include <Canvas.hpp>
#include <LinePlot.hpp>
#include <Color.hpp>
#include <Geometry.hpp>
#include <HorizontalLayout.hpp>
#include <cmath>

#define NUM_POINTS 100

int main()
{
	double x[NUM_POINTS];
	double y[NUM_POINTS];
	
	for (int i = 0; i < NUM_POINTS; ++i) {
		x[i] = i / static_cast<double>(NUM_POINTS);
		y[i] = sin(10.0 * x[i]);
	}
	
	hpl::Canvas canvas("../fonts/inconsolata.font");
    canvas.setBackgroundColor(hpl::Color(0.9f, 0.9f, 0.9f));
    canvas.setLayout(new hpl::HorizontalLayout());

    hpl::LinePlot* plot = canvas.addLinesPlot(NUM_POINTS, x, y);
    plot->setLegendColor(hpl::Color(0.0f, 0.0f, 0.0f));
    plot->setLineColor(0, hpl::Color(0.1f, 0.3f, 0.6f));
	
	canvas.wait();

    return 0;
}

