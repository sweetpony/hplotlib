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



#endif

