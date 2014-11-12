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
	
    hpl::Canvas canvas/*("../fonts/inconsolata.font")*/;

    hpl::OGLPlotter plotter;
    canvas.connectToPlotter(&plotter);
    plotter.setBackgroundColor(hpl::Color(0.9f, 0.9f, 0.9f));

    hpl::HorizontalLayout& layout1 = canvas.addLayout<hpl::HorizontalLayout>();
    hpl::VerticalLayout& layout2 = canvas.addLayout<hpl::VerticalLayout>();

    hpl::CoordinateSystem& cs1 = canvas.addCoordinateSystem<hpl::CoordinateSystem>();
    cs1.setColor(hpl::Color(0.6f, 0.6f, 0.6f));
    hpl::Lines& plot1 = cs1.addPlot<hpl::Lines>(NUM_POINTS, x, y);
    plot1.setColor(hpl::Color(1.0f, 0.0f, 0.0f));

    hpl::CoordinateSystem& cs2 = canvas.addCoordinateSystem<hpl::CoordinateSystem>();
    hpl::Contour& plot2 = cs2.addPlot<hpl::Contour>(NUM_POINTS, x, x, m);
    
    hpl::CoordinateSystem& cs3 = canvas.addCoordinateSystem<hpl::CoordinateSystem>(); 
    hpl::Points& plot3 = cs3.addPlot<hpl::Points>(NUM_POINTS, x, z);
    plot3.setColor(hpl::Color(0.0f, 0.0f, 1.0f));
    hpl::Lines& plot4 = cs3.addPlot<hpl::Lines>(NUM_POINTS, x, y);
    plot4.setColor(hpl::Color(0.0f, 0.0f, 0.0f));
	
	canvas.addCoordinateSystemToLayout(cs1.id(), layout1.id());
    canvas.addLayoutToLayout(layout2.id(), layout1.id());
    canvas.addCoordinateSystemToLayout(cs2.id(), layout2.id());
    canvas.addCoordinateSystemToLayout(cs3.id(), layout2.id());

    hpl::sleep(1e6);

    plot2.setColorTable<hpl::ColorTable::Rainbow>(256);

    hpl::sleep(1e6);
    
    layout2.changeOrientation(hpl::VerticalLayout::BottomToTop);

    hpl::sleep(1e6);
    
    layout1.changeOrientation(hpl::HorizontalLayout::RightToLeft);
    plot2.setColorTable<hpl::ColorTable::RainbowBlack>(256);

    //canvas.saveToFile("testoutput");
    plotter.wait();

    return 0;
}

