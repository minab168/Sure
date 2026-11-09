#ifndef SURE_HARDWARE_MMIO_INC_HPP_
#define SURE_HARDWARE_MMIO_INC_HPP_

#include "../../Common/Operations/CE_BitField.hpp"
#include "../../Common/Traits/Base.hpp"
#include "../../Common/Utils/Assert.hpp"


namespace sure::hw {

    enum class MMIOAccessMode {
        ReadOnly = 0,
        WriteOnly,
        ReadWrite,
    };


    template<UWord ADDR, UIntegral T = UWord, MMIOAccessMode ACCESS = MMIOAccessMode::ReadWrite>
    class MMIO {
        static_assert(
            ADDR % sizeof(T) == 0,
            "MMIO: register address must be aligned to the register width (sizeof(T))."
        );

        using V_PtrT = volatile T*;

      public:

        FORCE_INLINE_ static T read() noexcept {
            SURE_STATIC_ASSERT(ACCESS != MMIOAccessMode::WriteOnly, "MMIO: Cannot read a write-only register.");
            return *reinterpret_cast<V_PtrT>(ADDR);
        }


        FORCE_INLINE_ static Void write(T value) noexcept {
            SURE_STATIC_ASSERT(ACCESS != MMIOAccessMode::ReadOnly, "MMIO: Cannot write a read-only register.");
            *reinterpret_cast<V_PtrT>(ADDR) = value;
        }


        template<typename FIELD>
        FORCE_INLINE_ static T read_field(const FIELD& field) noexcept {
            SURE_STATIC_ASSERT(
                (SameAs<T, typename FIELD::Type>),
                "MMIO: the BitField's underlying type must be identical to the register type T."
            );

            let reg = MMIO::read();
            return field.extract(reg);
        }


        template<typename FIELD>
        FORCE_INLINE_ static Void set_mask(const FIELD& field) noexcept {
            SURE_STATIC_ASSERT(
                (SameAs<T, typename FIELD::Type>),
                "MMIO: the BitField's underlying type must be identical to the register type T."
            );

            MMIO::write(field.mask());
        }


        /**
         * Updates a single field and leaves all other bits of the register unchanged.
         *
         * Performs exactly one read followed by one write of the whole register.
         * This is NOT atomic: an interrupt or another core that writes this register
         * between the read and the write will have its change overwritten.
         *
         * Do not use on registers whose read has side effects (e.g. read-to-clear)
         * or on write-1-to-clear fields, because the extra read and the write-back
         * of untouched bits can clear or change state unintentionally.
         *
         * Values that do not fit in the field are truncated silently.
         */
        template<typename FIELD>
        FORCE_INLINE_ static Void modify_field(const FIELD& field, T value) noexcept {
            SURE_STATIC_ASSERT(
                (SameAs<T, typename FIELD::Type>),
                "MMIO: the BitField's underlying type must be identical to the register type T."
            );


            let reg = MMIO::read();
            let new_val = field.write(reg, value);
            MMIO::write(new_val);
        }


        /**
         * @ref modify_field
         */
        template<typename FIELD>
        FORCE_INLINE_ static Void set_bits(const FIELD& field) noexcept {
            MMIO::modify_field<FIELD>(field, field.mask_no_shift());
        }


        /**
         * @ref modify_field
         */
        template<typename FIELD>
        FORCE_INLINE_ static Void clear_bits(const FIELD& field) noexcept {
            MMIO::modify_field<FIELD>(field, 0);
        }
    };
}


#endif // SURE_HARDWARE_MMIO_INC_HPP_