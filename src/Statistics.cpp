#include "Statistics.hpp"
#include <cmath>

namespace hpl {
double min(int size, double const* x)
{
	double xmin = INFINITY;
	for (int i = 0; i < size; ++i) {
		if (x[i] < xmin) {
			xmin = x[i];
		}
	}
	return xmin;
}

double max(int size, double const* x)
{
	double xmax = -INFINITY;
	for (int i = 0; i < size; ++i) {
		if (x[i] > xmax) {
			xmax = x[i];
		}
	}
	return xmax;
}
double minPos(int size, double const* x)
{
    double xmin = INFINITY;
    for (int i = 0; i < size; ++i) {
        if (x[i] < xmin && x[i] > 0.0) {
            xmin = x[i];
        }
    }
    return xmin;
}

double maxPos(int size, double const* x)
{
    double xmax = 0.0;
    for (int i = 0; i < size; ++i) {
        if (x[i] > xmax && x[i] > 0.0) {
            xmax = x[i];
        }
    }
    return xmax;
}

double interpolate(double x, double x1, double y1, double x2, double y2)
{
    double alpha = (x - x1) / (x2 - x1);
    return y1 + (y2 - y1) * alpha;
}

double* log(int n, const double* x)
{
    double* y = new double[n];

    for (int i = 0; i < n; i++) {
        y[i] = log10(x[i]);
    }

    return y;
}
}
