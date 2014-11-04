#ifndef PAINTSERVER_HPP
#define PAINTSERVER_HPP

#include "Plot.hpp"

namespace hpl {
class PaintServer
{

public:
    PaintServer();
    ~PaintServer();

    template <typename P>
    Plot::ID addPlot(const P* raw, double xmin, double xmax, double ymin, double ymax) {
        //! @todo create a new plot which is fitted in the given box
        //! -> (x-xmin)/(xmax-xmin) same for y
        //! -> for each value which is outside [0,1] do sth dependent which type of plot:
        //! For Points remove it
        //! For Lines calculate intersections with box boundaries, add appropriate points and remove the outliers
        int n;
        double* x;
        double* y;

        P* fitted = new P(n, x, y);

        return data.add(fitted);
    }

private:
    Registry<Plot> data;

};
}

#endif // PAINTSERVER_HPP
