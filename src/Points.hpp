#ifndef POINTS_H
#define POINTS_H

#include <map>
#include <vector>

#include "Drawable.hpp"

namespace hpl
{
struct SimplePoints {
    SimplePoints(int n, double const* x, double const* y, bool ownsX, bool ownsY) : _n(n), _x(x), _y(y), _ownsX(ownsX), _ownsY(ownsY) {}
    virtual ~SimplePoints() {
        if (_ownsX) {
            delete[] _x;
        }
        if (_ownsY) {
            delete[] _y;
        }
    }

    const int _n;
    const double* _x, * _y;
    const bool _ownsX, _ownsY;
};

class Points : public Drawable, private SimplePoints
{

public:
    enum Symbol {
        Dot,
        Plus,
        Cross,
        Asterisk,
        Diamond,
        Circle,
        CirclePlus,
        CircleCross,
        Triangle,
        DownwardTriangle,
        RightwardTriangle,
        LeftwardTriangle,
        Square,
        Hourglass,
        Bowtie,
        VerticalBar,
        HorizontalBar,
        FilledDiamond,
        FilledCircle,
        FilledTriangle,
        FilledDownwardTriangle,
        FilledRightwardTriangle,
        FilledLeftwardTriangle,
        FilledSquare,
        FilledHourglass,
        FilledBowtie,
        FilledVerticalBar,
        FilledHorizontalBar
    };

    Points(int n, double const* x, double const* y, const Limits& limits, bool ownsData = false) :
        Drawable(Type_Points, limits), SimplePoints(n, x, y, ownsData, ownsData), points(new SimplePoints(n, x, y, false, false)){}
    virtual ~Points() {
        delete points;
    }

    virtual inline int n() const {
        return points->_n;
    }

    virtual inline const double* x() const {
        return points->_x;
    }

    virtual inline const double* y() const {
        return points->_y;
    }

    virtual inline void setColor(const Color& c) {
        color = c;
        changed.invoke(plotId);
    }
    virtual inline Color getColor() const {
        return color;
    }

    inline void setSymbolSize(double thick) {
        size = thick;
        changed.invoke(plotId);
    }
    inline double getSymbolSize() const {
        return size;
    }

    void setSymbol(Symbol s);
    inline Symbol getSymbol() const {
        return symbol;
    }
    //! @todo how to handle filling of symbols?
    inline bool isFilledSymbol() const {
        return symbol >= FilledDiamond;
    }

    virtual void recalculateData();

protected:
    std::vector<std::pair<double, double> > getSymbolVertices() const;
    void setTypeForSymbol();

    SimplePoints* points;
    Color color = Color(0.0f, 0.0f, 0.0f);
    double size = 1.0;
    Symbol symbol = Dot;
    bool limitsInCalc = false;

    const unsigned int maxSymbolVertices = 64;

};
}

#endif // POINTS_H
