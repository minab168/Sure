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

        CONSTEXPR_ BitField(const U8 offset, const U8 width) noexcept :
            _width{width},
            _offset{offset}
        {
            SURE_ABORT_ON_FALSE(bitfield_fits<T>(width, offset), "BitField range is invalid for the underlying type.");
        }

        NODISCARD_ CONSTEXPR_ T mask() const noexcept {
            return bitfield_mask_no_offset<T>(this->_width) << this->_offset;
        }

        NODISCARD_ CONSTEXPR_ T mask_no_offset() const noexcept {
            return bitfield_mask_no_offset<T>(this->_width);
        }

        NODISCARD_ CONSTEXPR_ T write(T reg, T value) const noexcept {
            return bitfield_write(reg, value, this->_offset, this->_width);
        }

        NODISCARD_ CONSTEXPR_ T extract(T reg) const noexcept {
            return bitfield_extract(reg, this->_offset, this->_width);
        }

        NODISCARD_ CONSTEXPR_ Bool is_valid(T value) const noexcept {
            return bitfield_is_valid(value, this->_width);
        }
    };

}

#endif // SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_