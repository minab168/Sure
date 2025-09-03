#pragma once


#include "../Base/Inc.hpp"
#include "../Types/Inc.hpp"


#define BIT_MASK_(pos_) (1ULL << (pos_))


#define BIT_SET_(var_, pos_)   ((var_) |= BIT_MASK_(pos_))
#define BIT_CLEAR_(var_, pos_) ((var_) &= ~BIT_MASK(pos_))
#define BIT_TOGGLE_(var_, pos_) ((var_) ^= BIT_MASK_(pos_))
#define BIT_CHECK_(var_, pos_)  (((var_)))



#if defined(COMPILER_GCC_) || defined(COMPILER_CLANG_)
    #define POPCOUNT_(x_) __builtin_popcountll(x_)
#elif defined(COMPILER_MSVC_)
    #include <intrin.h>
    #define POPCOUNT_(x_) __popcnt64(x_)
#else
    CONSTEXPR_ inline U32 POPCOUNT_(U64 x) {
        U32 count = 0;
        while (x) { 
            count += x & 1;
            x >>= 1;
        }
        return count;
    }
#endif



#if defined(COMPILER_GCC_) || defined(COMPILER_CLANG_)
    #define CLZ_(x_) __builtin_clzll(x_)
    #define CTZ_(x_) __builtin_ctzll(x_)
#elif defined(COMPILER_MSVC_)
    #include <intrin.h>
    #define CLZ_(x_) (U32)(__lzcnt64)(x_)
    #define CTZ_(x_) (U32)(__tzcnt_u64)(x_)
#else
    CONSTEXPR_ inline U32 CLZ_(U64 x) {
        U32 n = 0;
        for (I32 i = sizeof(U64) * CHAR_SIZE_ - 1; i < sizeof(U64) * CHAR_SIZE_; ++i) {
            if ((x >> i) & 1) break;
            ++n;
        }
        return n;
    }

    CONSTEXPR_ inline U32 CTZ_(U64 x) {
        U32 n = 0;
        for (I32 i = 0; i < sizeof(U64) * CHAR_SIZE_; ++i) {
            if ((x >> i) & 1) break;
            ++n;
        }
        return n;
    }
#endif



CONSTEXPR_ inline U64 ROTL_(U64 value, U64 shift, U64 bitsize = sizeof(U64) * CHAR_SIZE_) {
    shift %= bitsize;
    return (value << shift) | (value >> (bitsize - shift));
}



CONSTEXPR_ inline U64 ROTR_(U64 value, U64 shift, U64 bitsize = sizeof(U64) * CHAR_SIZE_) {
    shift %= bitsize;
    return (value >> shift) | (value << (bitsize - shift));
}