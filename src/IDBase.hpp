#ifndef IDBASE_HPP
#define IDBASE_HPP

#include <limits>

namespace hpl
{
template<typename StubForTypeSafety, typename T = unsigned>
class IDBase
{
public:
	typedef T Type;

	IDBase() : id(Invalid) {}
	IDBase(Type other) {
		id = other;
	}
	IDBase& operator=(Type other) {
		id = other;
		return *this;
	}
	operator Type() const { return id; }
	IDBase& operator++() {
		++id;
		return *this;
	}
	bool valid() const { return id != Invalid; }
	void invalidate() { id = Invalid; }

private:
	static const Type Invalid = std::numeric_limits<Type>::max();
    Type id;
};
}

#endif
