#ifndef SURE_HARDWARE_MMIO_INC_HPP_
#define SURE_HARDWARE_MMIO_INC_HPP_

#include "../../Common/Operations/BitField.hpp"
#include "../../Common/Traits/Base.hpp"
#include "../../Common/Utils/Assert.hpp"


namespace sure::hw {

    enum class MMIOAccessMode {
        ReadOnly = 0,
        WriteOnly,
        ReadWrite,
    };


    template<UIntegral T, Size ADDR, MMIOAccessMode ACCESS = MMIOAccessMode::ReadWrite>
    class MMIO {
        static_assert(
            ADDR % sizeof(T) == 0,
            "MMIO: register address must be aligned to the register width (sizeof(T))."
        );

        using V_PtrT = volatile T*;

      public:

        static T read() noexcept {
            SURE_STATIC_ASSERT(ACCESS != MMIOAccessMode::WriteOnly, "MMIO: Cannot read a write-only register.");
            return *reinterpret_cast<V_PtrT>(ADDR);
        }


        template<typename FIELD>
        static T read_field() noexcept {
            SURE_STATIC_ASSERT(
                (SameAs<T, typename FIELD::Type>),
                "MMIO: the BitField's underlying type must be identical to the register type T."
            );

            let reg = MMIO::read();
            return FIELD::extract(reg);
        }


        static Void write(T value) noexcept {
            SURE_STATIC_ASSERT(ACCESS != MMIOAccessMode::ReadOnly, "MMIO: Cannot write a read-only register.");
            *reinterpret_cast<V_PtrT>(ADDR) = value;
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
        static Void modify_field(T value) noexcept {
            SURE_STATIC_ASSERT(
                (SameAs<T, typename FIELD::Type>),
                "MMIO: the BitField's underlying type must be identical to the register type T."
            );


            let reg = MMIO::read();
            let new_val = FIELD::write(reg, value);
            MMIO::write(new_val);
        }


        /**
         * @ref modify_field
         */
        template<typename FIELD>
        static Void set_bits() noexcept {
            MMIO::modify_field<FIELD>(FIELD::mask());
        }


        /**
         * @ref modify_field
         */
        template<typename FIELD>
        static Void clear_bits() noexcept {
            MMIO::modify_field<FIELD>(0);
        }
    };
}


#endif // SURE_HARDWARE_MMIO_INC_HPP_