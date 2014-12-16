#ifndef HPLOTLIB_STATISTICS_HPP
#define HPLOTLIB_STATISTICS_HPP

namespace hpl
{
	double min(int size, double const* x);
	double max(int size, double const* x);

    double minPos(int size, double const* x);
    double maxPos(int size, double const* x);

    double interpolate(double x, double x1, double y1, double x2, double y2);

    double* log(int size, double const* x);
}

#endif
