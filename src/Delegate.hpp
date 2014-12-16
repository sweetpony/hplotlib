#ifndef DELEGATE_HPP
#define DELEGATE_HPP

template<typename... Arg>
class Delegate
{
	static inline void NullFunction(void*, Arg... arg) {}

	template<void (*Function)(Arg...)>
	static inline void DummyFunction(void*, Arg... arg) {
		return (*Function)(arg...);
	}
	
	template<class C, void (C::*Function)(Arg...)>
	static inline void DummyClassFunction(void* instance, Arg... arg) {
		return (static_cast<C*>(instance)->*Function)(arg...);
	}
	
public:
	template<void (*Function)(Arg...)>
	inline void bind() {
		function = &DummyFunction<Function>;
	}
	
	template<class C, void (C::*Function)(Arg...)>
	inline void bind(C* instance) {
		inst = instance;
		function = &DummyClassFunction<C, Function>;
    }

	inline void invoke(Arg... arg) {
		return (*function)(inst, arg...);
	}
	
	inline void* instance() const {
		return inst;
	}

private:
	void* inst = nullptr;
	void (*function)(void*, Arg...) = &Delegate::NullFunction;
};

#include <vector>

//! @todo merge into old Delegate
template<typename... Arg>
class MultiDelegate
{
    static inline void NullFunction(void*, Arg... arg) {}

    template<void (*Function)(Arg...)>
    static inline void DummyFunction(void*, Arg... arg) {
        return (*Function)(arg...);
    }

    template<class C, void (C::*Function)(Arg...)>
    static inline void DummyClassFunction(void* instance, Arg... arg) {
        return (static_cast<C*>(instance)->*Function)(arg...);
    }

public:
    template<void (*Function)(Arg...)>
    inline void bind() {
    }

    template<class C, void (C::*Function)(Arg...)>
    inline void bind(C* instance) {
        instances.push_back(instance);
        functions.push_back(&DummyClassFunction<C, Function>);
    }

    template<class C>
    inline void unbind(C* instance) {
        for (unsigned int i = 0; i < instances.size(); i++) {
            if (instances[i] == instance) {
                instances.erase(instances.begin() + i);
                functions.erase(functions.begin() + i);
                break;
            }
        }
    }

    inline void invoke(Arg... arg) {
        for (unsigned int i = 0; i < functions.size(); i++) {
            (functions[i])(instances[i], arg...);
        }
    }

private:
    std::vector<void*> instances;
    std::vector<void (*)(void*, Arg...)> functions;
};


#endif

