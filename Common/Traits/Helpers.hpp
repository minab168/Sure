#pragma once


#include "Base.hpp"

template<typename T, typename...>
using FirstT = T;

template<bool Cond, typename T = void>
using EnableIfT = typename EnableIf<Cond, T>::type;

template<typename... Bn>
auto or_fn__(int) -> FirstT<FalseType, EnableIfT<!bool(Bn::value)>...>;

template<typename... _Bn>
auto or_fn__(...) -> TrueType;

template<typename... Bn>
struct or__: decltype(or_fn__<Bn...>(0)) { };


template<typename T>
struct IsVoid: public FalseType { };

template<>
struct IsVoid<void>: public TrueType { };

template<>
struct IsVoid<const void>: public TrueType { };

template<>
struct IsVoid<volatile void>: public TrueType { };

template<>
struct IsVoid<const volatile void>: public TrueType { };


template<typename>
struct IsConst: public FalseType { };

template<typename T>
struct IsConst<T const>: public TrueType { };


template<typename T>
struct IsArrayUnknownBounds: public FalseType { };

template<typename T>
struct IsArrayUnknownBounds<T[]>: public TrueType { };


template<typename T>
struct IsFunction: public BoolConstant<!IsConst<const T>::value> { };

template<typename T>
struct IsFunction<T&>: public FalseType { };

template<typename T>
struct IsFunction<T&&>: public FalseType { };

template<typename T>
struct IsReference: public FalseType { };

template<typename T>
struct IsReference<T&>: public TrueType { };

template<typename T>
struct IsReference<T&&>: public TrueType { };

template<typename _Tp>
struct IsArithmetic: public or__<std::is_integral<_Tp>, std::is_floating_point<_Tp>>::type { };

template<typename _Tp>
struct IsEnum: public std::integral_constant<bool, __is_enum(_Tp)> { };

template<typename>
struct IsPointerHelper: public FalseType { };

template<typename T>
struct IsPointerHelper<T*>: public TrueType { };

template<typename T>
struct IsPointer: public IsPointerHelper<RemoveCVT<T>>::type { };

template<typename T>
struct IsMemberPointerHelper: public FalseType { };

template<typename T, typename C>
struct IsMemberPointerHelper<T C::*>: public TrueType { };

template<typename T>
struct IsMemberPointer: public IsMemberPointerHelper<RemoveCVT<T>>::type { };

template<typename _Tp>
struct IsNullPointer: public FalseType { };

template<>
struct IsNullPointer<std::nullptr_t>: public TrueType { };

template<>
struct IsNullPointer<const std::nullptr_t>: public TrueType { };

template<>
struct IsNullPointer<volatile std::nullptr_t>: public TrueType { };

template<>
struct IsNullPointer<const volatile decltype(NULL_PTR)>: public TrueType { };

template<typename T>
struct IsScalar: public or__<
    IsArithmetic<T>,
    IsEnum<T>,
    IsPointer<T>,
    IsMemberPointer<T>,
    IsNullPointer<T>
>::type { };


template<
    typename T,
    bool = or__<IsVoid<T>,
            IsArrayUnknownBounds<T>,
            std::is_function<T>>::value,
    bool = or__<IsReference<T>, IsScalar<T>>::value>
struct IsNtDestructibleSafe;


template<typename _Tp>
  struct IsNtDestructibleSafe<_Tp, false, false>
  : public __is_nt_destructible_impl<typename
             remove_all_extents<_Tp>::type>::type
{ };

template<typename T>
struct IsNtDestructibleSafe<T, true, false>: public FalseType { };

template<typename T>
struct IsNtDestructibleSafe<T, false, true>: public TrueType { };

template <typename T>
struct TypeIdentify { using type = T; };

template <typename T, Size = sizeof(T)>
constexpr TrueType IsCompleteOrUnbounded(TypeIdentify<T>) { return {}; }

template <
    typename TypeIdentity,
    typename NestedType = typename TypeIdentity::type>
constexpr typename or__<
    IsReference<NestedType>,
    IsFunction<NestedType>,
    IsVoid<NestedType>,
    IsArrayUnknownBounds<NestedType>
>::type IsCompleteOrUnbounded(TypeIdentity) { return {}; }

template<typename T>
struct IsNothrowDestructible: public IsNtDestructibleSafe<T>::type {
    static_assert(
        IsCompleteOrUnbounded(TypeIdentify<T>{}),
        "template argument must be a complete class or an unbounded array"
    );
};


template<typename T>
struct IsDestructible: public std::__is_destructible_safe<T>::type {
    static_assert(
        IsCompleteOrUnbounded(TypeIdentify<T>{}),
        "template argument must be a complete class or an unbounded array"
    );
};


#define enable_if_(CondT_, T_) (EnableIf<CondT_, T_>::type)

#define is_derived_from_(DerivedT_, BaseT_) (IsBaseOf<BaseT_, DerivedT_>::value)

#define is_same_(T1_, T2_) (IsSameByRemovingRefConstVolatile<T1_, T2_>::value)

#define is_same_template_(T1_, T2_) (IsSameTemplate<T1_, T2_>::value)


#define is_primitive_type_(T_) ( \
    is_same_(T_, char) || is_same_(T_, int) || is_same_(T_, short) || is_same_(T_, long) || is_same_(T_, long long) || \
    is_same_(T_, unsigned char) || is_same_(T_, unsigned int) || is_same_(T_, unsigned short) || is_same_(T_, unsigned long) || \
    is_same_(T_, unsigned long long) || is_same_(T_, bool) || is_same_(T_, float) || is_same_(T_, double) || \
    is_same_(T_, const char) || is_same_(T_, const int) || is_same_(T_, const short) || is_same_(T_, const long) || is_same_(T_, const long long) || \
    is_same_(T_, const unsigned char) || is_same_(T_, const unsigned int) || is_same_(T_, const unsigned short) || is_same_(T_, const unsigned long) || \
    is_same_(T_, const unsigned long long) || is_same_(T_, const bool) || is_same_(T_, const float) || is_same_(T_, const double) || \
)

#define is_primitive_pointer_type_(T_)   ( \
    is_same_(T_, char*) || is_same_(T_, int*) || is_same_(T_, short*) || is_same_(T_, long*) || is_same_(T_, long long*) || is_same_(T_, void*) || \
    is_same_(T_, unsigned char*) || is_same_(T_, unsigned int*) || is_same_(T_, unsigned short*) || is_same_(T_, unsigned long*) || \
    is_same_(T_, unsigned long long*) || is_same_(T_, bool*) || is_same_(T_, float*) || is_same_(T_, double*) || \
    is_same_(T_, const char*) || is_same_(T_, const int*) || is_same_(T_, const short*) || is_same_(T_, const long*) || is_same_(T_, const long long*) || is_same_(T_, const void*) || \
    is_same_(T_, const unsigned char*) || is_same_(T_, const unsigned int*) || is_same_(T_, const unsigned short*) || is_same_(T_, const unsigned long*) || \
    is_same_(T_, const unsigned long long*) || is_same_(T_, const bool*) || is_same_(T_, const float*) || is_same_(T_, const double*) || \
    is_same_(T_, char const*) || is_same_(T_, int const*) || is_same_(T_, short const*) || is_same_(T_, long const*) || is_same_(T_, long long const*) || is_same_(T_, void const*) || \
    is_same_(T_, unsigned char const*) || is_same_(T_, unsigned int const*) || is_same_(T_, unsigned short const*) || is_same_(T_, unsigned long const*) || \
    is_same_(T_, unsigned long long const*) || is_same_(T_, bool const*) || is_same_(T_, float const*) || is_same_(T_, double const*) || \
)

#define is_derived_from_primitive_type_(T_)   ( \
    is_derived_from_(T_, char) || is_derived_from_(T_, int) || is_derived_from_(T_, short) || is_derived_from_(T_, long) || is_derived_from_(T_, long long) || \
    is_derived_from_(T_, unsigned char) || is_derived_from_(T_, unsigned int) || is_derived_from_(T_, unsigned short) || is_derived_from_(T_, unsigned long) || \
    is_derived_from_(T_, unsigned long long) || is_derived_from_(T_, bool) || is_derived_from_(T_, float) || is_derived_from_(T_, double) \
)


#define is_primitive_(T_) ( \
    is_primitive_type_(T_) || is_primitive_pointer_type_(T_) || is_derived_from_primitive_type_(T_) \
)
