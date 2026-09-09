#ifndef SURE_COMMON_BASE_OPERATIONS_BITWISE_HPP_
#define SURE_COMMON_BASE_OPERATIONS_BITWISE_HPP_

#include "../Base/Interface/Lang.hpp"
#include "../Types/Primitives/Inc.hpp"
#include "../Types/Primitives/Size.hpp"
#include "../Traits/Primitives.hpp"


namespace sure::base {

    /**
     * Performs a logical left shift on an unsigned integer value.
     *
     * NOTE: The caller is responsible for ensuring thet `offset` is less than
     * the bit width of `T`. Passing an offset equal to or greater than
     * the bit width of `T` results in undefined behaviour.
    */
    template<UIntegral T>
    CONSTEXPR_ T bit_lsh(T value, const U8 offset) noexcept {
        return static_cast<T>(value << offset);
    }

    /**
     * Performs a logical right shift on an unsigned integer value.
     *
     * NOTE: The caller is responsible for ensuring thet `offset` is less than
     * the bit width of `T`. Passing an offset equal to or greater than
     * the bit width of `T` results in undefined behaviour.
    */
    template<UIntegral T>
    CONSTEXPR_ T bit_rsh(T value, const U8 offset) noexcept {
        return static_cast<T>(value >> offset);
    }

    template<UIntegral T>
    CONSTEXPR_ T bit_set_mask(T value, T mask) noexcept {
        return static_cast<T>(value | mask);
    }

    template<UIntegral T>
    CONSTEXPR_ T bit_get_mask(T value, T mask) noexcept {
        return static_cast<T>(value & mask);
    }

    template<UIntegral T>
    CONSTEXPR_ T bit_clear_mask(T value, T mask) noexcept {
        return static_cast<T>(value & ~mask);
    }

    template<UIntegral T>
    CONSTEXPR_ T bit_toggle_mask(T value, T mask) noexcept {
        return static_cast<T>(value ^ mask);
    }

    template<UIntegral T>
    CONSTEXPR_ T bit_set(T value, const U8 pos) noexcept {
        return static_cast<T>(bit_set_mask<T>(value, static_cast<T>(1) << pos));
    }

    template<UIntegral T>
    CONSTEXPR_ T bit_get(T value, const U8 pos) noexcept {
        return static_cast<T>(bit_get_mask<T>(value >> pos, 1));
    }

    template<UIntegral T>
    CONSTEXPR_ T bit_clear(T value, const U8 pos) noexcept {
        return static_cast<T>(bit_clear_mask<T>(value, static_cast<T>(1) << pos));
    }

    template<UIntegral T>
    CONSTEXPR_ T bit_toggle(T value, const U8 pos) noexcept {
        return static_cast<T>(bit_toggle_mask<T>(value, static_cast<T>(1) << pos));
    }

    template<UIntegral T>
    CONSTEXPR_ Bool bit_check(T value, const U8 pos) noexcept {
        return bit_get<T>(value, pos) == 1;
    }


    #if defined(SURE__COMPILER_GCC_) || defined(SURE__COMPILER_CLANG_)
        #define pop_count_(x_) __builtin_popcountll(x_)
    #elif defined(SURE__COMPILER_MSVC_)
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


    #if defined(SURE__COMPILER_GCC_) || defined(SURE__COMPILER_CLANG_)
        #define clz(x_) __builtin_clzll(x_)
        #define ctz(x_) __builtin_ctzll(x_)
    #elif defined(SURE__COMPILER_MSVC_)
        #include <intrin.h>
        #define clz(x_) (U32)(__lzcnt64)(x_)
        #define ctz(x_) (U32)(__tzcnt_u64)(x_)
    #else
        template<UIntegral T>
        CONSTEXPR_ T clz(T x) {
            U32 n = 0;
            for (I32 i = static_cast<I32>(sizeof(T) * CHAR_SIZE_) - 1; i >= 0; --i) {
                if ((x >> i) & 1) break;
                ++n;
            }
            return n;
        }

        template<UIntegral T>
        CONSTEXPR_ T ctz(T x) {
            U32 n = 0;
            for (I32 i = 0; i < sizeof(T) * CHAR_SIZE_; ++i) {
                if ((x >> i) & 1) break;
                ++n;
            }
            return n;
        }
    #endif



    template<UIntegral T>
    CONSTEXPR_ T rotl(T value, T shift, T bitsize = sizeof(T) * CHAR_SIZE_) {
        shift %= bitsize;
        if (shift == 0) return value;
        return (value << shift) | (value >> (bitsize - shift));
    }



    template<UIntegral T>
    CONSTEXPR_ T rotr(T value, T shift, T bitsize = sizeof(T) * CHAR_SIZE_) {
        shift %= bitsize;
        if (shift == 0) return value;
        return (value >> shift) | (value << (bitsize - shift));
    }

}

#endif // SURE_COMMON_BASE_OPERATIONS_BITWISE_HPP_