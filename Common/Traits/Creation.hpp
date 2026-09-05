#pragma once
#include "Base.hpp"


template<typename T>
concept DefaultConstructible =
#if SURE__HAS_BUILTIN_(__is_constructible)
    __is_constructible(T);
#else
    requires { T(); T{}; };
#endif


template<typename T, typename ...Args>
concept Constructible =
#if SURE__HAS_BUILTIN_(__is_constructible)
    __is_constructible(T, Args...);
#else
    requires(Args&&... args) { T(static_cast<Args&&>(args)...); T{static_cast<Args&&>(args)...}; };
#endif



template<typename T>
concept Destructible = requires(T t) { { t.~T() } noexcept; };  // FIXME: use builtine


template<typename T>
concept CopyConstructible =
#if SURE__HAS_BUILTIN_(__is_constructible)
    __is_constructible(T, AddLvalueT<const T>);
#else
    requires(T t) { T(t); T{t}; };
#endif


template<typename T>
concept MoveConstructible =
#if SURE__HAS_BUILTIN_(__is_constructible)
    __is_constructible(T, AddRvalueT<const T>);
#else
        requires(T t) { T(static_cast<T&&>(t)); T{static_cast<T&&>(t)}; };
#endif



template<typename T>
concept CopyAssignable =
#if SURE__HAS_BUILTIN_(__is_assignable)
    __is_assignable(AddLvalueT<T>, AddLvalueT<const T>);
#else
        requires(T t, T u) { { t = u }; };
#endif



template<typename T>
concept MoveAssignable =
#if SURE__HAS_BUILTIN_(__is_assignable)
    __is_assignable(AddLvalueT<T>, AddRvalueT<T>);
#else
    requires(T t, T u) { { t = static_cast<T&&>(u) }; };
#endif



template<typename T>
concept Copyable = CopyConstructible<T> and CopyAssignable<T> and Destructible<T>;



template<typename T>
concept Movable = MoveConstructible<T> and MoveAssignable<T> and Destructible<T>;