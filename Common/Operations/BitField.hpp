#ifndef SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_
#define SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_

#include "Bitwise.hpp"


namespace sure {

    template<UIntegral T, U8 OFFSET, U8 WIDTH>
    class BitField {
        static_assert(WIDTH > 0, "Width must be greater than 0");
        static_assert(OFFSET + WIDTH <= sizeof(T) * CHAR_SIZE_, "BitField range exceeds the size of the underlying type.");

      public:
        static CONSTEXPR_ T mask() noexcept {
            if constexpr (sizeof(T) * CHAR_SIZE_ == WIDTH) {
                return ~static_cast<T>(0);
            }
            else {
                return (static_cast<T>(1) << WIDTH) - 1;
            }
        }

        static CONSTEXPR_ T write(T reg, T value) noexcept {
            T reg_cleared = bit_clear_mask<T>(reg, BitField::mask() << OFFSET);
            T value_shifted = bit_get_mask<T>(value, BitField::mask()) << OFFSET;
            return bit_set_mask<T>(reg_cleared, value_shifted);
        }

        static CONSTEXPR_ T extract(T reg) noexcept {
            return bit_get_mask<T>(reg >> OFFSET, BitField::mask());
        }

        static CONSTEXPR_ Bool is_valid(T value) noexcept {
            return (value | BitField::mask()) == BitField::mask();
        }
    };

}

#endif // SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_