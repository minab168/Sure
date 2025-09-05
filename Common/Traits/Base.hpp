#pragma once


#if __has_builtin(__remove_reference)
    template<typename T> struct RemoveRef { using type = __remove_reference(T); };
#else
    template<typename T> struct RemoveRef { using type = T; };
    template<typename T> struct RemoveRef<T&> { using type = T; };
    template<typename T> struct RemoveRef<T&&> { using type = T; };
#endif


template<typename T> struct RemoveConst { typedef T Type; };

template<typename T> struct RemoveConst<const T> { typedef T Type; };


template<typename T> struct AddConst { using type = const T; };


template<typename T> struct RemoveConstVolatile { typedef T Type; };

template<typename T> struct RemoveConstVolatile<const T> { typedef T Type; };

template<typename T> struct RemoveConstVolatile<volatile T> { typedef T Type; };

template<typename T> struct RemoveConstVolatile<const volatile T> { typedef T Type; };


struct FalseType { static constexpr bool value = false; };

struct TrueType  { static constexpr bool value = true; };


template<typename> struct IsLvalueRef: public FalseType { };

template<typename T> struct IsLvalueRef<T&>: public TrueType { };

template<typename> struct IsRvalueRef: public FalseType { };

template<typename T> struct IsRvalueRef<T&&>: public TrueType { };


template<typename, typename> struct IsSame: public FalseType {};

template<typename T> struct IsSame<T, T>: public TrueType  {};


template<template<typename...> class, template<typename...> class> struct IsSameTemplate: public FalseType {};
template<template<typename...> class T> struct IsSameTemplate<T, T>: public TrueType  {};


template <typename T, T V>
struct IntegralConstant {
    static constexpr T value = V;

    using ValueType = T;
    using Type       = IntegralConstant;

    constexpr operator ValueType() const noexcept { return value; }
    constexpr ValueType operator()() const noexcept { return value; }
};


template<bool, typename T = void> struct EnableIf { };

template<typename Base, typename Derived> struct IsBaseOf: public IntegralConstant<bool, __is_base_of(Base, Derived)> { };


template<typename T1, typename T2> struct IsSameByRemovingRefConstVolatile : public IsSame<
    typename RemoveRef<typename RemoveConstVolatile<T1>::Type>::Type,
    typename RemoveRef<typename RemoveConstVolatile<T2>::Type>::Type
> {};



template<typename T, typename U>
concept SameAs = requires { typename EnableIf<IsSameByRemovingRefConstVolatile<T, U>::value>; };