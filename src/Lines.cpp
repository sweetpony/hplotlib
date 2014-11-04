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
}

Lines::~Lines()
{
}

void Lines::resetData(int n, double const* x, double const* y, bool del)
{
    if (del) {
        delete[] this->x;
        delete[] this->y;
    }

    this->n = n;
    this->x = x;
    this->y  =y;

    changed.invoke();
}
}
