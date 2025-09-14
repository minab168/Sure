#pragma once


class NoDefaultCopy {
    constexpr NoDefaultCopy(const NoDefaultCopy&) noexcept = delete;
    constexpr NoDefaultCopy& operator=(NoDefaultCopy&) noexcept = delete;
  public:
    constexpr NoDefaultCopy() noexcept = default;
    virtual constexpr ~NoDefaultCopy() noexcept = default;
    constexpr NoDefaultCopy(NoDefaultCopy&&) noexcept = default;
    constexpr NoDefaultCopy& operator=(NoDefaultCopy&&) noexcept = default;
};