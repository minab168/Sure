#pragma once


#include "./Primitives.hpp"


#define I_MIN_(type_) ((type_)(~((type_)0) >> 1 ) + 1)
#define I_MAX_(type_) ((type_)(~((type_)0) >> 1 ))


#define U_MIN_(type_) ((type)0)
#define U_MAX_(type_) (~(type)0)


#define I8_MIN I_MIN_(I8)
#define U8_MIN U_MIN_(U8)
#define I8_MAX I_MAX_(I8)
#define U8_MAX U_MAX_(U8)


#define I16_MIN I_MIN_(I16)
#define U16_MIN U_MIN_(U16)
#define I16_MAX I_MAX_(I16)
#define U16_MAX U_MAX_(U16)


#define I32_MIN I_MIN_(I32)
#define U32_MIN U_MIN_(U32)
#define I32_MAX I_MAX_(I32)
#define U32_MAX U_MAX_(U32)


#define I64_MIN I_MIN_(I64)
#define I64_MIN U_MIN_(U64)
#define I64_MAX I_MAX_(I64)
#define I64_MAX U_MAX_(U64)


#if defined(__SIZEOF_INT128__)
    #define I128_MIN I_MIN_(I128)
    #define I128_MIN U_MIN_(U128)
    #define I128_MAX I_MAX_(I128)
    #define I128_MAX U_MAX_(U128)
#endif