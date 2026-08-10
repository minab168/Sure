#pragma once


#include "Move.hpp"


class NoDefaultCopy {
  public:
    constexpr NoDefaultCopy(const NoDefaultCopy&) noexcept = delete;
    constexpr NoDefaultCopy& operator=(NoDefaultCopy&) noexcept = delete;
    constexpr NoDefaultCopy() noexcept = default;

#ifndef NO_RUNTIME_
    virtual
#endif
    constexpr ~NoDefaultCopy() noexcept = default;

    constexpr NoDefaultCopy(NoDefaultCopy&&) noexcept = default;
    constexpr NoDefaultCopy& operator=(NoDefaultCopy&&) noexcept = default;
};