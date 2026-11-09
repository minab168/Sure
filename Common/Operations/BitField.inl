#ifndef SURE_COMMON_BASE_OPERATIONS_BITFIELD_INL_
#define SURE_COMMON_BASE_OPERATIONS_BITFIELD_INL_

#if defined(SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_) && defined(SURE_COMMON_BASE_OPERATIONS_CE_BITFIELD_HPP_)
    #error "Do not include this file directly"
#endif


namespace sure {

    template<typename T>
    FORCE_INLINE_ CONSTEXPR_ T bitfield_mask_no_offset(U8 width) noexcept {
        if (sizeof(T) * CHAR_SIZE_ == width) {
            return ~static_cast<T>(0);
        }

        return (static_cast<T>(1) << width) - 1;
    }


    template<typename T>
    FORCE_INLINE_ CONSTEXPR_ T bitfield_write(T reg, T value, const U8 offset, const U8 width) noexcept {
        T reg_cleared = bit_clear_mask<T>(reg, bitfield_mask_no_offset<T>(width) << offset);
        T value_shifted = bit_get_mask<T>(value, bitfield_mask_no_offset<T>(width)) << offset;
        return bit_set_mask<T>(reg_cleared, value_shifted);
    }


    template<typename T>
    FORCE_INLINE_ CONSTEXPR_ T bitfield_extract(T reg, const U8 offset, const U8 width) noexcept {
        return bit_get_mask<T>(reg >> offset, bitfield_mask_no_offset<T>(width));
    }


    template<typename T>
    FORCE_INLINE_ CONSTEXPR_ Bool bitfield_is_valid(T value, U8 width) noexcept {
        return (value | bitfield_mask_no_offset<T>(width)) == bitfield_mask_no_offset<T>(width);
    }


    template<typename T>
    FORCE_INLINE_ CONSTEXPR_ Bool bitfield_fits(const U8 offset, const U8 width) noexcept {
        return width > 0 && (offset + width ) <= sizeof(T) * CHAR_SIZE_;
    }

}

#endif // SURE_COMMON_BASE_OPERATIONS_BITFIELD_INL_
