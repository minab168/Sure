#ifndef SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_
#define SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_

#include "../Base/Lang.hpp"
#include "../Types/Primitives/Size.hpp"

namespace sure::base {

    template<typename T, T OFFSET, T WIDTH>
    class BitField {
        static_assert(OFFSET + WIDTH <= sizeof(T) * CHAR_SIZE_);

        static CONSTEXPR_ T mask() noexcept {
            // return
        }

        static T write(T reg, T value) noexcept {

        }
    };

}

#endif // SURE_COMMON_BASE_OPERATIONS_BITFIELD_HPP_