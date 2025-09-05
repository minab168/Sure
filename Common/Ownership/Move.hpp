#pragma once


#include "../Traits/Base.hpp"


template<typename T>
NODISCARD_
constexpr typename RemoveRef<T>::type&& do_move(T&& obj) noexcept {
    return static_cast<typename RemoveRef<T>::type&&>(obj);
}


template<typename T>
NODISCARD_
constexpr typename RemoveRef<T>::Type&& do_move(const T& obj) noexcept {
    return do_move( const_cast<T&>(obj) );
}


template<typename T>
NODISCARD_
constexpr T&& do_forward(typename RemoveRef<T>::Type& obj) noexcept {
    return static_cast<T&&>(obj);
}


template<typename T>
NODISCARD_
constexpr T&& do_forward(typename RemoveRef<T>::Type&& obj) noexcept {
    static_assert(!IsLvalueRef<T>::value, "Invalid rvalue to lvalue conversion!");
    return static_cast<T&&>(obj);
}


