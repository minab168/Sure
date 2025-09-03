#pragma once


#include "Base.hpp"


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
