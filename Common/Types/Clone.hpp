#pragma once


#include "../Base/Compiler.hpp"


template<typename Derived>
class Clone {
  protected:
    virtual Derived _clone_impl() const& noexcept = 0;

  public:
    constexpr Clone() noexcept = default;

    virtual constexpr ~Clone() noexcept = default;

    NODISCARD_
    Derived clone() const& noexcept;
};


// =======================================================


template<typename Derived>
Derived Clone<Derived>::clone() const& noexcept {
    return this->_clone_impl();
}