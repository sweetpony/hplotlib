#ifndef AXISFLAGS_HPP
#define AXISFLAGS_HPP

namespace hpl {
namespace AxisFlags {
    constexpr int PaintNothing = 0;
    constexpr int PaintPrimary = 1 << 0;
    constexpr int PaintSecondary = 1 << 1;
    constexpr int Logscale = 1 << 2;
    constexpr int PaintMinorTicks = 1 << 3;

    enum AxisOrientation {
        Horizontal,
        Vertical
    };

    enum TickMode {
        FixedAmount,
        Smart,
        Fixed
    };
}
}

#endif // AXISFLAGS_HPP
