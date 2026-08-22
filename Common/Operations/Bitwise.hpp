#pragma once


#include "../Types/Inc.hpp"


namespace sure::opr::bitwise {

    template<typename T>
    CONSTEXPR_ T mask(const U8 offset, const U8 width) noexcept {
        return ((static_cast_(U8, 1) << width) - 1) << offset;
    }

    template<typename T>
    CONSTEXPR_ T bit_mask(const U8 pos) noexcept {
        return mask<T>(pos, 1);
    }

    template<typename T>
    CONSTEXPR_ T bit_set(T value, const U8 pos) noexcept {
        return static_cast_(T, value | bit_mask<T>(pos));
    }

    template<typename T>
    CONSTEXPR_ T bit_get(T value, const U8 pos) noexcept {
        return static_cast<T>(
            value & (static_cast<T>(1) << pos)
        );
    }

    template<typename T>
    CONSTEXPR_ T bit_set_many(T value, const U8 offset, const U8 width, T field) noexcept {
        const T mask_val = mask<T>(offset, width);
        return static_cast<T>(
            (value & ~mask_val) |
            ((field << offset) & mask)
        );
    }

    template<typename T>
    CONSTEXPR_ T bit_get_many(T value, const U8 width, const U8 offset) noexcept {
        return static_cast<T>(
            (value >> offset) & ((static_cast<T>(1) << width) - 1)
        );
    }

    template<typename T>
    CONSTEXPR_ T bit_clear(T value, const U8 pos) noexcept {
        return static_cast_(T, value & ~bit_mask<T>(pos));
    }

    template<typename T>
    CONSTEXPR_ T bit_toggle(T value, const U8 pos) noexcept {
        return static_cast_(T, value ^ ~bit_mask<T>(pos));
    }

    template<typename T>
    CONSTEXPR_ T bit_check(T value, const U8 pos) noexcept {
        return static_cast_(T, value & bit_mask<T>(pos)) != 0;
    }

    template<typename T>
    CONSTEXPR_ T bit_any(T value, T mask) noexcept {
        return (value & mask) != 0;
    }

    template<typename T>
    CONSTEXPR_ T bit_all(T value, T mask) noexcept {
        return (value & mask) == mask;
    }


    #if defined(SURE__COMPILER_GCC_) || defined(SURE__COMPILER_CLANG_)
        #define pop_count_(x_) __builtin_popcountll(x_)
    #elif defined(COMPILER_MSVC_)
        #include <intrin.h>
        #define pop_count_(x_) __popcnt64(x_)
    #else
        CONSTEXPR_ U32 pop_count_(U64 x) {
            U32 count = 0;
            while (x) {
                count += x & 1;
                x >>= 1;
            }
            return count;
        }
    #endif


    #if defined(COMPILER_GCC_) || defined(COMPILER_CLANG_)
        #define clz(x_) __builtin_clzll(x_)
        #define ctz(x_) __builtin_ctzll(x_)
    #elif defined(COMPILER_MSVC_)
        #include <intrin.h>
        #define clz(x_) (U32)(__lzcnt64)(x_)
        #define ctz(x_) (U32)(__tzcnt_u64)(x_)
    #else
        CONSTEXPR_ U32 clz(U64 x) {
            U32 n = 0;
            for (I32 i = sizeof(U64) * CHAR_SIZE_ - 1; i < sizeof(U64) * CHAR_SIZE_; ++i) {
                if ((x >> i) & 1) break;
                ++n;
            }
            return n;
        }

        CONSTEXPR_ U32 ctz(U64 x) {
            U32 n = 0;
            for (I32 i = 0; i < sizeof(U64) * CHAR_SIZE_; ++i) {
                if ((x >> i) & 1) break;
                ++n;
            }
            return n;
        }
    #endif



    CONSTEXPR_ U64 rotl(U64 value, U64 shift, U64 bitsize = sizeof(U64) * CHAR_SIZE_) {
        shift %= bitsize;
        return (value << shift) | (value >> (bitsize - shift));
    }



    CONSTEXPR_ U64 rotr(U64 value, U64 shift, U64 bitsize = sizeof(U64) * CHAR_SIZE_) {
        shift %= bitsize;
        return (value >> shift) | (value << (bitsize - shift));
    }

}