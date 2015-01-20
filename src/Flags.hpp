#ifndef FLAGSHPP
#define FLAGSHPP

namespace hpl
{

template <typename Enum>
class Flags
{

public:
    typedef decltype(Enum()|Enum()) StoredEnum;

    Flags();
    Flags(StoredEnum flags);

    inline operator StoredEnum() const {
        return flags;
    }
    bool operator[](Enum flag) const;

    Flags& operator()(Enum flag, bool val = true);
    Flags<Enum>& operator&=(const Flags<Enum>& rhs);
    Flags<Enum>& operator|=(const Flags<Enum>& rhs);
    Flags<Enum>& operator^=(const Flags<Enum>& rhs);

    Flags& reset();

    Flags& invert();
    Flags& invert(Enum flag);

    inline bool any() const {
        return flags > 0;
    }

    inline bool none() const {
        return flags == 0;
    }

private:
    StoredEnum flags;

};
}

#include "Flags.inl"

#endif // FLAGSHPP
