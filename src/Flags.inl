#include "Flags.hpp" //Just for IDE to know the class definitions

namespace hpl {

// Macro to define function bodies for binary operators
#define FlagsOperator(OP) \
    template<typename Enum> \
    Flags<Enum>& Flags<Enum>::operator OP ## =(const Flags<Enum>& rhs) \
    { \
        this->flags = this->flags OP rhs.flags; \
        return *this; \
    } \
\
    template<typename Enum> \
    Flags<Enum> Flags<Enum>::operator OP(const Flags<Enum>& rhs) const \
    { \
        Flags<Enum> res(*this); \
        return res OP ## = rhs; \
    } \
\
    template<typename Enum> \
    Flags<Enum> Flags<Enum>::operator OP(const Enum& rhs) const \
    { \
        return (*this) OP Flags<Enum>(rhs); \
    } \
\
    template<typename Enum> \
    Flags<Enum> operator OP(const Enum& lhs, const Flags<Enum>& rhs) \
    { \
        return rhs OP Flags<Enum>(lhs); \
    }

template <typename Enum>
Flags<Enum>::Flags() : flags(StoredEnum(0))
{
}

template <typename Enum>
Flags<Enum>::Flags(Enum e) : flags(e)
{
}

template <typename Enum>
Flags<Enum>::Flags(const std::initializer_list<Enum>& initList)
{
    flags = std::accumulate(initList.begin(), initList.end(), StoredEnum(0), [](StoredEnum x, StoredEnum y) { return x | y; });
}

template <typename Enum>
bool Flags<Enum>::operator[](Enum flag) const
{
    return flags & flag;
}

template <typename Enum>
Flags<Enum>& Flags<Enum>::operator()(Enum flag, bool val)
{
    flags = (val ? (flags|flag) : (flags&~flag));
    return *this;
}

FlagsOperator(&)
FlagsOperator(|)
FlagsOperator(^)

template<typename Enum>
Flags<Enum>& Flags<Enum>::reset()
{
    flags = StoredEnum(0);
    return *this;
}

template<typename Enum>
Flags<Enum>& Flags<Enum>::invert()
{
    flags = ~flags;
    return *this;
}

template<typename Enum>
Flags<Enum>& Flags<Enum>::invert(Enum flag)
{
    flags ^= flag;
    return *this;
}

}
