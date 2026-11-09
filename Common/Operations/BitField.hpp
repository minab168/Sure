#ifndef SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_
#define SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_

#include "Bitwise.hpp"
#include "./BitField.inl"
#include "../Utils/Assert.hpp"

namespace sure {

    template<UIntegral T>
    class BitField {

        U8 _width;
        U8 _offset;

      public:
        using Type = T;

        BitField(const U8 offset, const U8 width) noexcept :
            _width{width},
            _offset{offset}
        {
            SURE_ABORT_ON_FALSE(bitfield_fits<T>(width, offset), "BitField range is invalid for the underlying type.");
        }

        CONSTEXPR_ T mask() noexcept {
            return bitfield_mask<T>(this->_width);
        }

        CONSTEXPR_ T write(T reg, T value) noexcept {
            return bitfield_write(reg, value, this->_width, this->_offset);
        }

        CONSTEXPR_ T extract(T reg) noexcept {
            return bitfield_extract(reg, this->_width, this->_offset);
        }

        CONSTEXPR_ Bool is_valid(T value) noexcept {
            return bitfield_is_valid(value, this->_width);
        }
    };

}

#endif // SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_