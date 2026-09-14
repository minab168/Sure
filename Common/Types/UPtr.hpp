/*
 * ──────────────────────────────────────────────
 * Project   : Sure
 * File      : UPtr
 * Author    : Emad Redwan
 * Created   : 9/5/25
 * License   : MIT
 * ──────────────────────────────────────────────
 */


#pragma once
#include "../Base/Interface/Lang.hpp"
#include "../Ownership/Inc.hpp"


using sure::ownership::NoDefaultCopy;


template<typename PtrT>
class UPtr final: NoDefaultCopy {
    PtrT _ptr = NULL_PTR;

    constexpr explicit UPtr(PtrT ptr) noexcept;

  public:
    constexpr UPtr() noexcept = default;

    constexpr UPtr(UPtr&&) noexcept;

    constexpr UPtr& operator=(UPtr&&) noexcept;
};


// ==============================================================


template <typename PtrT>
constexpr UPtr<PtrT>::UPtr(PtrT ptr) noexcept:
    _ptr(ptr)
{ }


template <typename PtrT>
constexpr UPtr<PtrT>::UPtr(UPtr&& obj) noexcept:
    _ptr(obj._ptr)
{
    obj._ptr = NULL_PTR;
}


template <typename PtrT>
constexpr UPtr<PtrT>& UPtr<PtrT>::operator=(UPtr&& obj) noexcept {
    this._ptr = obj._ptr;
    obj._ptr = NULL_PTR;
    return *this;
}
