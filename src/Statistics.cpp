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
}
