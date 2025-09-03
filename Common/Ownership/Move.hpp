#pragma once


#include "../Traits/Base.hpp"


template<typename T>
constexpr typename RemoveRef<T>::Type&& do_move(T&& obj) noexcept {
    return static_cast<typename RemoveRef<T>::Type&&>(obj);
}


template<typename T>
constexpr typename RemoveRef<T>::Type&& do_move(const T& obj) noexcept {
    return obj_move( const_cast<T&>(obj) );
}


template<typename T>
constexpr T&& do_forward(typename RemoveRef<T>::Type& obj) noexcept {
    return static_cast<T&&>(obj);
}


template<typename T>
constexpr T&& do_forward(typename RemoveRef<T>::Type&& obj) noexcept {
    static_assert(!IsLvalueRef<T>::value, "Invalid rvalue to lvalue conversion!");
    return static_cast<T&&>(obj);
}


