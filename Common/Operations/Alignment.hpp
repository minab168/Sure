#ifndef SURE_COMMON_BASE_OPERATIONS_ALIGNMENT_HPP_
#define SURE_COMMON_BASE_OPERATIONS_ALIGNMENT_HPP_


#include "Traits/Primitives.hpp"

namespace sure {

    template<UIntegral T>
    CONSTEXPR_ T align_up(T value, T alignment) noexcept {
        return (value + alignment -1 ) & ~(alignment - 1);
    }

    template<UIntegral T>
    CONSTEXPR_ T align_up_general(T value, T alignment) noexcept {
        return ((value + alignment - 1) / alignment) * alignment;
        // return (value / alignment) + (value % alignment == 0 ? 0 : 1) * alignment;
    }

    template<UIntegral T>
    CONSTEXPR_ T is_aligned(T value, T alignment) noexcept {
        return value % alignment == 0;
    }

}


#endif //SURE_COMMON_BASE_OPERATIONS_ALIGNMENT_HPP_