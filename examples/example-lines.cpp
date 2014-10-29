/*
 * lines.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */

#include <cmath>

#include <Canvas.hpp>
#include <Plot.hpp>
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
    hpl::HorizontalLayout& layout1 = canvas.addLayout<hpl::HorizontalLayout>();
    hpl::VerticalLayout& layout2 = canvas.addLayout<hpl::VerticalLayout>();

    hpl::Plot& plot1 = canvas.add1D<hpl::Lines>(NUM_POINTS, x, y);
    plot1.setLegendColor(hpl::Color(0.0f, 0.0f, 0.0f));
    plot1.setColor(0, hpl::Color(1.0f, 0.0f, 0.0f));

    hpl::Plot& plot2 = canvas.add1D<hpl::Lines>(NUM_POINTS, x, z);
    plot2.setLegendColor(hpl::Color(0.0f, 0.0f, 0.0f));
    plot2.setColor(0, hpl::Color(0.0f, 1.0f, 0.0f));

    hpl::Plot& plot3 = canvas.add1D<hpl::Points>(NUM_POINTS, x, z);
    plot3.setLegendColor(hpl::Color(0.0f, 0.0f, 0.0f));
    plot3.setColor(0, hpl::Color(0.0f, 0.0f, 1.0f));
    hpl::PlotPart::ID id3b = plot3.addPlotPart<hpl::Lines>(NUM_POINTS, x, y);
    plot3.setColor(id3b, hpl::Color(0.0f, 0.0f, 0.0f));
	
	canvas.addPlotToLayout(plot1.id(), layout1.id());
	canvas.addLayoutToLayout(layout2.id(), layout1.id());
	canvas.addPlotToLayout(plot2.id(), layout2.id());
	canvas.addPlotToLayout(plot3.id(), layout2.id());

    hpl::sleep(1e6);
    
    layout2.changeOrientation(hpl::VerticalLayout::BottomToTop);

    hpl::sleep(1e6);
    
    layout1.changeOrientation(hpl::HorizontalLayout::RightToLeft);

    /*canvas.setLayout(new hpl::HorizontalLayout(hpl::HorizontalLayout::RightToLeft));
    
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

    canvas.saveToFile("testoutput");*/
	
    canvas.wait();

    return 0;
}

