/*
 * lines.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */

#include <cmath>

#include <Canvas.hpp>
#include <Plot2D.hpp>
#include <Color.hpp>
#include <Geometry.hpp>
#include <HorizontalLayout.hpp>
#include <VerticalLayout.hpp>
#include <GridLayout.hpp>

#define NUM_POINTS 200

int main()
{
	double x[NUM_POINTS];
	double y[NUM_POINTS];
    double z[NUM_POINTS];
	
	for (int i = 0; i < NUM_POINTS; ++i) {
        x[i] = i / static_cast<double>(NUM_POINTS);
		y[i] = sin(10.0 * x[i]);
        z[i] = cos(20.0 * x[i]);
	}
	
	hpl::Canvas canvas("../fonts/inconsolata.font");
    canvas.setBackgroundColor(hpl::Color(0.9f, 0.9f, 0.9f));
    canvas.setLayout(new hpl::HorizontalLayout());

    hpl::Plot2D* plot1 = canvas.add2DPlot<hpl::Lines>(NUM_POINTS, x, y);
    plot1->setLegendColor(hpl::Color(0.0f, 0.0f, 0.0f));
    plot1->setColor(0, hpl::Color(0.1f, 0.3f, 0.6f));

    hpl::sleep(1e6);

    hpl::Plot2D* plot2 = canvas.add2DPlot<hpl::Lines>(NUM_POINTS, x, z);
    plot2->setLegendColor(hpl::Color(0.0f, 0.0f, 0.0f));
    plot2->setColor(0, hpl::Color(0.6f, 0.3f, 0.1f));

    hpl::sleep(1e6);

    canvas.setLayout(new hpl::HorizontalLayout(hpl::HorizontalLayout::RightToLeft));
    
    for (int i = 0; i < 5; ++i) {
		hpl::sleep(5e5);
        plot2->setColor(0, hpl::Color(
			rand() / static_cast<float>(RAND_MAX), 
			rand() / static_cast<float>(RAND_MAX), 
			rand() / static_cast<float>(RAND_MAX)
		));
    }

    hpl::sleep(1e6);

    canvas.setLayout(new hpl::GridLayout(2, 2, hpl::GridLayout::BottomRightToTopLeft));

    hpl::Plot2D* plot3 = canvas.add2DPlot<hpl::Points>(NUM_POINTS, x, y);
    plot3->setLegendColor(hpl::Color(0.0f, 0.0f, 0.0f));
    plot3->setColor(0, hpl::Color(0.0f, 0.0f, 0.0f));

    hpl::sleep(1e6);

    static_cast<hpl::GridLayout*>(canvas.getLayout())->changeOrientation(hpl::GridLayout::TopLeftToBottomRight);

    canvas.saveToFile("testoutput");
	
    canvas.wait();

    return 0;
}

