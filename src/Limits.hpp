#ifndef LIMITS_HPP
#define LIMITS_HPP

#include <cmath>

#include "Delegate.hpp"

namespace hpl {
class Limits
{
public:
    Limits() {}
    Limits(double xmin, double xmax, double ymin, double ymax) : _xmin(xmin), _xmax(xmax), _ymin(ymin), _ymax(ymax) {}

    inline void setLimits(double xmin, double xmax, double ymin, double ymax) {
        this->_xmin = xmin;
        this->_xmax = xmax;
        this->_ymin = ymin;
        this->_ymax = ymax;
        changed.invoke();
    }
    inline void setLimits(const Limits& limits) {
        this->_xmin = limits.xmin();
        this->_xmax = limits.xmax();
        this->_ymin = limits.ymin();
        this->_ymax = limits.ymax();
        changed.invoke();
    }
    inline void setXLimits(double xmin, double xmax) {
        this->_xmin = xmin;
        this->_xmax = xmax;
        changed.invoke();
    }
    inline void setXLimits(const Limits& limits) {
        this->_xmin = limits.xmin();
        this->_xmax = limits.xmax();
        changed.invoke();
    }
    inline void setYLimits(double ymin, double ymax) {
        this->_ymin = ymin;
        this->_ymax = ymax;
        changed.invoke();
    }
    inline void setYLimits(const Limits& limits) {
        this->_ymin = limits.ymin();
        this->_ymax = limits.ymax();
        changed.invoke();
    }

    inline double xmin() const {
        return _xmin;
    }
    inline double xmax() const {
        return _xmax;
    }
    inline double ymin() const {
        return _ymin;
    }
    inline double ymax() const {
        return _ymax;
    }

    inline bool xValid() const {
        return _xmin != std::numeric_limits<double>::min() && _xmax != std::numeric_limits<double>::max();
    }
    inline bool yValid() const {
        return _ymin != std::numeric_limits<double>::min() && _ymax != std::numeric_limits<double>::max();
    }
    inline bool valid() const {
        return xValid() && yValid();
    }

    MultiDelegate<> changed;

private:
    double _xmin = std::numeric_limits<double>::min();
    double _xmax = std::numeric_limits<double>::max();
    double _ymin = std::numeric_limits<double>::min();
    double _ymax = std::numeric_limits<double>::max();
};
}

#endif // LIMITS_HPP
