#pragma once


#include "./Platform.hpp"


#ifdef __cplusplus
    #define NULL_PTR nullptr
#else
    #define NULL_PTR ((void*)0)
#endif


#define let const auto
#define mut auto


#define loop while(true)

#define comma_(a_, b_)          a_, b_
#define max_(val1_, val2_)      ((val1_) > (val2_) ? (val1_) : (val2_))
#define min_(val1_, val2_)      ((val1_) < (val2_) ? (val1_) : (val2_))
#define array_len_(array_name_) (sizeof(array_name_) / sizeof(*array_name_))


#if IS_CPP_ 
    #define CONSTEXPR_ constexpr
#else 
    #define CONSTEXPR_
#endif


#if defined(COMPILER_GCC_) || defined(COMPILER_CLANG_)
    #define todo_(msg_) do_pragma_(message("TODO: " #msg_))
#else
    #define todo_(msg_)
#endif


#if defined(COMPILER_GCC_) || defined(COMPILER_CLANG_)
    #define UNSAFE_ __attribute__((error("This is an unsafe call. wrap it with `unsafe_block_` macro")))
#elif defined(COMPILER_MSVC_)
    #define UNSAFE_ __declspec(deprecated("This is an unsafe call. wrap it with `unsafe_block_` macro"))
#else
    #pragma message("Warning: `UNSAFE_` macro isn ot supported on your compiler")
    #define UNSAFE_
#endif


/* #if defined(COMPILER_GCC_) || defined(COMPILER_CLANG_)
//     #define unsafe_block_(block_) \
//         #pragma GCC diagnostic push \
//         #pragma GCC diagnostic ignored "-Wattributes" \
//         block_ \
//         #pragma GCC diagnostic pop
// #elif defined(COMPILER_MSVC_)
//     #define unsafe_block_(block_) \
//         __pragma(warning(push)) \
//         __pragma(warning(disable:4996)) \
//         block_ \
//         __pragma(warning(pop)) \
// #else
//     #pragma message("Warning: `unsafe_block_` macro isn ot supported on your compiler")
//     #define unsafe_block_(block_)
// #endif */