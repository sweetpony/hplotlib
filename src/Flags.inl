#include "Flags.hpp" //Just for IDE to know the class definitions

namespace hpl {

template <typename Enum>
Flags<Enum>::Flags() : flags(StoredEnum(0))
{
}

/*template <typename Enum>
Flags<Enum>::Flags(StoredEnum flags) : flags(flags)
{
}*/

template <typename Enum>
Flags<Enum>::Flags(Enum e) : flags(e)
{
}

template <typename Enum>
Flags<Enum>::Flags(const std::initializer_list<Enum>& initList)
{
    flags = std::accumulate(initList.begin(), initList.end(), StoredEnum(0), [](Enum x, Enum y) { return x | y; });
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

template<typename Enum>
Flags<Enum>& Flags<Enum>::operator&=(const Flags<Enum>& rhs)
{
    this->flags = this->flags & rhs.flags;
    return *this;
}

template<typename Enum>
Flags<Enum> Flags<Enum>::operator&(const Flags<Enum>& rhs) const
{
    Flags<Enum> res(*this);
    res &= rhs;
    return res;
}

template<typename Enum>
Flags<Enum> Flags<Enum>::operator&(const Enum& rhs) const
{
    return (*this) & Flags<Enum>(rhs);
}

template<typename Enum>
Flags<Enum>& Flags<Enum>::operator|=(const Flags<Enum>& rhs)
{
    this->flags = this->flags | rhs.flags;
    return *this;
}

template<typename Enum>
Flags<Enum> Flags<Enum>::operator|(const Flags<Enum>& rhs) const
{
    Flags<Enum> res(*this);
    res |= rhs;
    return res;
}

template<typename Enum>
Flags<Enum> Flags<Enum>::operator|(const Enum& rhs) const
{
    return (*this) | Flags<Enum>(rhs);
}

template<typename Enum>
Flags<Enum>& Flags<Enum>::operator^=(const Flags<Enum>& rhs)
{
    this->flags = this->flags ^ rhs.flags;
    return *this;
}

template<typename Enum>
Flags<Enum> Flags<Enum>::operator^(const Flags<Enum>& rhs) const
{
    Flags<Enum> res(*this);
    res ^= rhs;
    return res;
}

template<typename Enum>
Flags<Enum> Flags<Enum>::operator^(const Enum& rhs) const
{
    return (*this) ^ Flags<Enum>(rhs);
}

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
