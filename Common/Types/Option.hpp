#pragma once


#include "./Primitives.hpp"
#include "./Clone.hpp"
#include "../Traits/Inc.hpp"


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
class Option: NoDefaultCopy, public Clone {
    Bool   _has_value = false;
    ValueT _value{};   

    constexpr Option(ValueT&& val) noexcept;

    Option _clone_impl() const noexcept override;

  public:
    constexpr Option() noexcept = default;

    constexpr Option(Option&&) noexcept;

    static Option some(ValueT&& val) noexcept;

    static Option none() noexcept;

    static Option from(const Option& obj) noexcept;

    NODISCARD_
    Bool is_some() const noexcept;

    NODISCARD_
    Bool is_none() const noexcept;

    NODISCARD_
    ValueT&& value_or_default() && noexcept;

    Option& operator=(Option&&) noexcept;
};


// ========================================================================


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
inline Option<ValueT> Option<ValueT>::some(ValueT&& val) noexcept {
    return Option(do_move(val));
};


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
inline Option<ValueT> Option<ValueT>::_clone_impl() const noexcept {
    static_assert(Clonable<ValueT>, "Option value type (ValueT) must be Clonable!");
    return this->_has_value ? Option(this->_value.clone()) : Option();
};


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
constexpr Option<ValueT>::Option(Option<ValueT>&& obj) noexcept :
    _has_value(obj._has_value),
    _value(do_move(obj._value))
{ }


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
constexpr Option<ValueT>::Option(ValueT&& val) noexcept :
    _has_value(true),
    _value(do_move(val))
{ }


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
inline Option<ValueT> Option<ValueT>::none() noexcept {
    return Option();
};


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
inline Option<ValueT> Option<ValueT>::from(const Option<ValueT>& obj) noexcept {
    static_assert(From<ValueT>, "Option value type (ValueT) must implement `From`!");
    return obj._has_value ? Option(obj._value.clone()) : Option();
};


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
inline Bool Option<ValueT>::is_some() const noexcept {
    return this->_has_value;
};


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
inline Bool Option<ValueT>::is_none() const noexcept {
    return !this->_has_value;
};


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
inline ValueT&& Option<ValueT>::value_or_default() && noexcept {
    return this->_has_value ? do_move(this->_value) : ValueT();
};


template<typename ValueT>
    requires DefaultConstructible<ValueT> and Movable<ValueT>
inline Option<ValueT>& Option<ValueT>::operator=(Option<ValueT>&& obj) noexcept {
    this->_has_value = obj._has_value;
    this->_value     = do_move(obj._value);
    return *this;
};