#ifndef SURE_COMMON_BASE_OPERATIONS_CE_BITFIELD_HPP_
#define SURE_COMMON_BASE_OPERATIONS_CE_BITFIELD_HPP_

#include "Bitwise.hpp"
#include "./BitField.inl"

namespace sure {

    template<UIntegral T, U8 OFFSET, U8 WIDTH>
    class CE_BitField {
        static_assert(bitfield_fits<T>(WIDTH, OFFSET), "BitField range is invalid for the underlying type.");

      public:
        using Type = T;

        static CONSTEXPR_ T mask() noexcept {
            return bitfield_mask<T>(WIDTH);
        }

        static CONSTEXPR_ T write(T reg, T value) noexcept {
            return bitfield_write(reg, value, WIDTH, OFFSET);
        }

        static CONSTEXPR_ T extract(T reg) noexcept {
            return bitfield_extract(reg, WIDTH, OFFSET);
        }

        static CONSTEXPR_ Bool is_valid(T value) noexcept {
            return bitfield_is_valid(value, WIDTH);
        }
    };

}

#endif // SURE_COMMON_BASE_OPERATIONS_CE_BITFIELD_HPP_