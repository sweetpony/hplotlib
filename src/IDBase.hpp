#ifndef IDBASE_HPP
#define IDBASE_HPP

namespace hpl
{
template<typename StubForTypeSafety, typename T = unsigned>
class IDBase
{
public:
	typedef T Type;
	IDBase() : id(-1) {}
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

private:
    Type id;
};
}

#endif
