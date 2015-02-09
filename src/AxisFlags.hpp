#ifndef AXISFLAGS_HPP
#define AXISFLAGS_HPP

namespace hpl {
namespace AxisFlags {

    enum AxisFlags {
        PaintNothing = 0,
        PaintPrimary = 1 << 0,
        PaintSecondary = 1 << 1,
        Logscale = 1 << 2,
        PaintMinorTicks = 1 << 3,
        PaintLabelsPrimary = 1 << 4,
        PaintLabelsSecondary = 1 << 5
    };

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
