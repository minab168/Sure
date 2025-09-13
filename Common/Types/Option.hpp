#pragma once


#include "Primitives/Inc.hpp"
#include "./Clone.hpp"
#include "../Ownership/Inc.hpp"
#include "../Traits/Inc.hpp"
#include "../Utils/Abort.hpp"


template<typename RefT>
  requires (not Reference<RefT>)
class OptionRef final: NoDefaultCopy {
    Bool _has_value = false;
    RefT* _ptr = NULL_PTR;

    constexpr explicit OptionRef(RefT& ref) noexcept;


  public:
    constexpr OptionRef() noexcept = default;

    constexpr OptionRef(OptionRef&&) noexcept;

    static OptionRef some(RefT& ref) noexcept;

    static OptionRef none() noexcept;

    NODISCARD_
    Bool is_some() const& noexcept;

    NODISCARD_
    Bool is_none() const& noexcept;

    NODISCARD_
    RefT& value_or_abort() const&& noexcept;

    NODISCARD_
    RefT& value_or_abort() && noexcept;

    OptionRef& operator=(OptionRef&&) noexcept;
};


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
class Option final: NoDefaultCopy, public Clone<Option<ValueT>> {
    Bool   _has_value = false;
    ValueT _value{};

    constexpr explicit Option(const ValueT& val) noexcept;

    constexpr explicit Option(ValueT&& val) noexcept;

    Option _clone_impl() const& noexcept override;

  public:
    constexpr Option() noexcept = default;

    constexpr Option(Option&&) noexcept;

    static Option some(ValueT&& val) noexcept;

    static Option none() noexcept;

    static Option from(const Option& obj) noexcept;

    NODISCARD_
    Bool is_some() const& noexcept;

    NODISCARD_
    Bool is_none() const& noexcept;

    NODISCARD_
    ValueT value_or_default() && noexcept;

    NODISCARD_
    ValueT value_or_abort() && noexcept;

    NODISCARD_
    ValueT take() & noexcept;

    NODISCARD_
    OptionRef<const ValueT> as_ref() const& noexcept
    requires (not Reference<ValueT>);

    NODISCARD_
    OptionRef<ValueT> as_mut() & noexcept
    requires (not Reference<ValueT>);

    Option& operator=(Option&&) noexcept;
};


// ========================================================================


template<typename RefT>
  requires (not Reference<RefT>)
constexpr OptionRef<RefT>::OptionRef(RefT& ref) noexcept:
    _has_value(true),
    _ptr(&ref)
{ }


template<typename RefT>
  requires (not Reference<RefT>)
constexpr OptionRef<RefT>::OptionRef(OptionRef&& obj) noexcept:
    _has_value(true),
    _ptr(obj._ptr)
{
    obj._ptr = nullptr;
}


template<typename RefT>
  requires (not Reference<RefT>)
OptionRef<RefT> OptionRef<RefT>::some(RefT& ref) noexcept {
    return OptionRef(ref);
}


template<typename RefT>
  requires (not Reference<RefT>)
OptionRef<RefT> OptionRef<RefT>::none() noexcept {
    return OptionRef();
}


template<typename RefT>
  requires (not Reference<RefT>)
Bool OptionRef<RefT>::is_some() const& noexcept {
    return this->_has_value;
}


template<typename RefT>
  requires (not Reference<RefT>)
Bool OptionRef<RefT>::is_none() const& noexcept {
    return !this->_has_value;
}


template<typename RefT>
  requires (not Reference<RefT>)
RefT& OptionRef<RefT>::value_or_abort() const&& noexcept {
    if (!this->_has_value) {
        abort_("The given option has no value!");
    }
    return *this->_ptr;
}


template<typename RefT>
  requires (not Reference<RefT>)
RefT& OptionRef<RefT>::value_or_abort() && noexcept {
    if (!this->_has_value) {
        abort_("The given option has no value!");
    }
    return *this->_ptr;
}


template<typename RefT>
  requires (not Reference<RefT>)
OptionRef<RefT>& OptionRef<RefT>::operator=(OptionRef&& obj) noexcept {
    this->_has_value = obj._has_value;
    this->_ptr       = obj._ptr;
    return *this;
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
Option<ValueT> Option<ValueT>::some(ValueT&& val) noexcept {
    return Option(do_move(val));
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
Option<ValueT> Option<ValueT>::_clone_impl() const& noexcept {
    if constexpr (Clonable<ValueT>) {
        return this->_has_value ? Option(this->_value.clone()) : Option();
    }
    else if constexpr (Copyable<ValueT>) {
        return this->_has_value ? Option(do_move(this->_value)) : Option();
    }
    else {
        abort_("Type of value in the option isn't `Copyable` or doesn't implement `Clonable`!");
    }
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
constexpr Option<ValueT>::Option(Option&& obj) noexcept :
    _has_value(obj._has_value),
    _value(do_move(obj._value))
{ }


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
constexpr Option<ValueT>::Option(const ValueT& val) noexcept :
    _has_value(true),
    _value(val)
{ }


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
constexpr Option<ValueT>::Option(ValueT&& val) noexcept :
    _has_value(true),
    _value(do_move(val))
{ }


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
Option<ValueT> Option<ValueT>::none() noexcept {
    return Option();
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
Option<ValueT> Option<ValueT>::from(const Option& obj) noexcept {
    static_assert(From<ValueT>, "Option value type (ValueT) must implement `From`!");
    return obj._has_value ? Option(obj._value.clone()) : Option();
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
Bool Option<ValueT>::is_some() const& noexcept {
    return this->_has_value;
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
Bool Option<ValueT>::is_none() const& noexcept {
    return !this->_has_value;
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
ValueT Option<ValueT>::value_or_default() && noexcept {
    return this->_has_value ? do_move(this->_value) : ValueT();
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
ValueT Option<ValueT>::value_or_abort() && noexcept {
    if (!this->_has_value) {
        abort_("The given option has no value!");
    }
    return do_move(this->_value);
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
ValueT Option<ValueT>::take() & noexcept {
    this->_has_value = false;
    return do_move(this->_value);
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
OptionRef<const ValueT> Option<ValueT>::as_ref() const& noexcept
  requires (not Reference<ValueT>)
{
    return this->_has_value ? OptionRef<const ValueT>::some(this->_value) : OptionRef<const ValueT>::none();
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
OptionRef<ValueT> Option<ValueT>::as_mut() & noexcept
  requires (not Reference<ValueT>)
{
    return this->_has_value ? OptionRef<ValueT>::some(this->_value) : OptionRef<ValueT>::none();
}


template<typename ValueT>
  requires DefaultConstructible<ValueT> and Movable<ValueT>
Option<ValueT>& Option<ValueT>::operator=(Option&& obj) noexcept {
    this->_has_value = obj._has_value;
    this->_value     = do_move(obj._value);
    return *this;
}


// ========================================================================


#define some_(val_, ty_) Option<ty_>::some(val_)
#define none_(ty_)       Option<ty_>::none()