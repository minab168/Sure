#ifndef SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_
#define SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_

#include "../Base/Lang.hpp"
#include "../Types/Primitives/Inc.hpp"
#include "../Types/Primitives/Size.hpp"
#include "../Traits/Primitives.hpp"


namespace sure::base {

    template<UIntegral T, U8 OFFSET, U8 WIDTH>
    class BitField {
        static_assert(OFFSET + WIDTH <= sizeof(T) * CHAR_SIZE_);

      public:
        static CONSTEXPR_ T mask() noexcept {
            if constexpr (sizeof(T) == WIDTH) {
                return ~static_cast<T>(0);
            }
            else {
                return (static_cast<T>(1) << WIDTH) - 1;
            }
        }

        static T write(T reg, T value) noexcept {
            let reg_cleared   = reg & ~(BitField::mask() << OFFSET);
            let value_shifted = (value & BitField::mask()) << OFFSET;
            return reg_cleared | value_shifted;
        }

        static T extract(T reg) noexcept {
            return (reg & BitField::mask() << OFFSET) >> OFFSET;
        }

        static Bool is_enabled(T reg) noexcept {
            return (reg & BitField::mask() << OFFSET) == 1;
        }

        static Bool is_disabled(T reg) noexcept {
            return (reg & BitField::mask() << OFFSET) == 0;
        }

        static Bool is_valid(T value) noexcept {
            return (value | BitField::mask()) == BitField::mask();
        }
    };

}

#endif // SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_