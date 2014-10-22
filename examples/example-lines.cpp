/*
 * lines.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */

#include <Canvas.hpp>
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
	canvas.addLinesPlot(NUM_POINTS, x, y, 0.1, 0.3, 1.0, 0.0, 0.0, 1.0, 1.0);
	
	canvas.wait();

    return 0;
}

