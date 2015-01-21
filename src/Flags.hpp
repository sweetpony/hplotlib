#ifndef FLAGSHPP
#define FLAGSHPP

#include <initializer_list>
#include <numeric>

namespace hpl
{

template <typename Enum>
class Flags
{

public:
    typedef decltype(Enum()|Enum()) StoredEnum;

    Flags();
    //! @todo need to make it work without this to ensure typesafety
    //Flags(StoredEnum flags);
    Flags(Enum e);
    //! @todo how to pull this outside into .inl?
    template <typename ...T>
    Flags(Enum e, T... t) : Flags(t...) {
        flags = flags | e;
    }
    Flags(const std::initializer_list<Enum>& initList);

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
