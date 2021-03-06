#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <unordered_map>

namespace hpl
{
template<typename T>
class Registry
{
public:
	typedef typename std::unordered_map<typename T::ID, T*, std::hash<typename T::ID::Type>> ObjectMap;
	typedef typename ObjectMap::const_iterator const_iterator;

	~Registry() {
		for (auto it = objects.cbegin(); it != objects.cend(); ++it) {
			delete it->second;
		}
	}

    inline bool has(typename T::ID id) const {
		return objects.count(id) > 0;
	}
	
	inline T& lookup(typename T::ID id) {
		return *objects[id];
    }
    inline const T& lookup(typename T::ID id) const {
        return *objects.at(id);
    }
	
	inline typename T::ID add(T* object) {
		typename T::ID id = nextID;
		++nextID;
		object->setId(id);
		objects[id] = object;
		return id;
	}
	
	inline void remove(typename T::ID id) {
		T& o = lookup(id);
		objects.erase(id);
		delete &o;		
	}

    inline const_iterator begin() noexcept {
        return objects.begin();
    }
	inline const_iterator cbegin() const noexcept {
		return objects.cbegin();
	}

    inline const_iterator end() noexcept {
        return objects.end();
    }
	inline const_iterator cend() const noexcept {
		return objects.cend();
	}
private:
    typename T::ID nextID = typename T::ID(static_cast<typename T::ID::Type>(0));
	ObjectMap objects;
};
}

#endif
