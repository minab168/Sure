#ifndef SURE_COMMON_OWNERSHIP_MOVE_HPP_
#define SURE_COMMON_OWNERSHIP_MOVE_HPP_


#include "../Base/Compiler.hpp"
#include "../Traits/Base.hpp"


template<typename T>
NODISCARD_
constexpr typename RemoveRef<T>::Type&& do_move(T&& obj) noexcept {
    return static_cast<typename RemoveRef<T>::Type&&>(obj);
}


template<typename T>
NODISCARD_
constexpr typename RemoveConst<typename RemoveRef<T>::Type>::Type&& do_move(const T& obj) noexcept {
    using Raw = typename RemoveConst<typename RemoveRef<T>::Type>::Type;
    return static_cast<Raw&&>(const_cast<Raw&>(obj));
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


#endif // SURE_COMMON_OWNERSHIP_MOVE_HPP_