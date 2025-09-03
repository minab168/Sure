#pragma once



template<typename Derived>
class Clone {
  protected:
    virtual Derived _clone_impl() const noexcept = 0;

  public:
    constexpr Clone() noexcept = default;

    constexpr ~Clone() noexcept = default;

    NODISCARD_
    Derived clone() const noexcept;
};


// =======================================================


template<typename Derived>
inline Derived Clone<Derived>::clone() const noexcept {
    return this->_clone_impl(derived);
}