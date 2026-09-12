//
// Created by user on 12/09/2026.
//

#ifndef SURE_COMMON_TYPES_ARRAY_HPP
#define SURE_COMMON_TYPES_ARRAY_HPP

#include "../Ownership/Inc.hpp"
#include "../Ownership/Move.hpp"
#include "../Traits/Creation.hpp"
#include "Primitives/Inc.hpp"


namespace sure {

    template<typename T, Size N>
    requires DefaultConstructible<T>
    class Array: NoDefaultCopy {
        T _data[N];

        constexpr Array() noexcept = default;

    public:
        template<typename ...Args>
        requires (sizeof...(Args) == N)
        explicit Array(Args&&...args) noexcept:
            _data{static_cast<T>(do_forward<Args>(args))...}
        { }

    };

}


#endif // SURE_COMMON_TYPES_ARRAY_HPP