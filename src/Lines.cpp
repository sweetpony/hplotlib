/*
 * Lines.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: Carsten Uphoff (uphoff@mytum.de)
 */
#include "Lines.hpp"

namespace hpl
{
Lines::Lines(int n, double const* x, double const* y) : n(n), x(x), y(y)
{
    xmin = hpl::min(n, x);
    xmax = hpl::max(n, x);
    ymin = hpl::min(n, y);
    ymax = hpl::max(n, y);

    /*interleave = new float[2*n];
    for (int i = 0; i < n; ++i) {
        interleave[(i << 1)] = (x[i] - xmin) / (xmax - xmin);
        interleave[(i << 1) + 1] = (y[i] - ymin) / (ymax - ymin);
    }*/
}

Lines::~Lines()
{
    //delete[] interleave;
}
}
