#pragma once



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