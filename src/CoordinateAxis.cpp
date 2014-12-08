#include "CoordinateAxis.hpp"

namespace hpl {

template<>
CoordinateAxis<Horizontal>::CoordinateAxis(Registry<Drawable>& data, std::map<Drawable::ID, unsigned int>& dataRevisions) :
    offset(0.12f), data(data), dataRevisions(dataRevisions)
{
}

template<>
CoordinateAxis<Vertical>::CoordinateAxis(Registry<Drawable>& data, std::map<Drawable::ID, unsigned int>& dataRevisions) :
    offset(0.08f), data(data), dataRevisions(dataRevisions)
{
}

}
