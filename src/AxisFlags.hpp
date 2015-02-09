#ifndef AXISFLAGS_HPP
#define AXISFLAGS_HPP

namespace hpl {

enum AxisFlags {
    PaintNothing = 0,
    PaintPrimary = 1 << 0,
    PaintSecondary = 1 << 1,
    Logscale = 1 << 2,
    PaintMinorTicks = 1 << 3,
    PaintLabelsPrimary = 1 << 4,
    PaintLabelsSecondary = 1 << 5
};

enum class AxisOrientation {
    Horizontal,
    Vertical
};

enum class TickMode {
    FixedAmount,
    Smart,
    Fixed
};

}

#endif // AXISFLAGS_HPP
