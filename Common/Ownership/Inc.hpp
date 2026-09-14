#ifndef SURE_COMMON_OWNERSHIP_INC_HPP_
#define SURE_COMMON_OWNERSHIP_INC_HPP_


#include "../Base/Interface/Runtime.hpp"


namespace sure {
    
    class NoDefaultCopy {
      public:
        constexpr NoDefaultCopy(const NoDefaultCopy&) noexcept = delete;
        constexpr NoDefaultCopy& operator=(NoDefaultCopy&) noexcept = delete;
        constexpr NoDefaultCopy() noexcept = default;

        VIRTUAL_IF_RUNTIME_
        constexpr ~NoDefaultCopy() noexcept = default;

        constexpr NoDefaultCopy(NoDefaultCopy&&) noexcept = default;
        constexpr NoDefaultCopy& operator=(NoDefaultCopy&&) noexcept = default;
    };
}


#endif // SURE_COMMON_OWNERSHIP_INC_HPP_