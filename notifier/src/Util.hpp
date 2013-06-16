#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <cassert>

template <typename T>
struct Ref {
	explicit Ref(T& arg) :
		mRef(&arg) {
	}

	operator T&() const {
		return *mRef;
	}

private:
	T* mRef;
};

template <typename T>
Ref<T> make_cref(T& arg) {
	return Ref<const T>(arg);
}

#endif
