/*
 * ──────────────────────────────────────────────
 * Project   : Sure
 * File      : Result
 * Author    : Emad Redwan
 * Created   : 9/4/25
 * License   : MIT
 * ──────────────────────────────────────────────
 */


#pragma once


#include "Clone.hpp"
#include "Option.hpp"
#include "Variant.hpp"
#include "../Ownership/Inc.hpp"
#include "../Traits/Clone.hpp"
#include "../Traits/Creation.hpp"
#include "Primitives/Inc.hpp"


template<typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
class Result final: NoDefaultCopy, Clone<Result<ValueT, ErrorT>> {
    Bool                    _is_err = false;
    Variant<ValueT, ErrorT> _value{};

    constexpr explicit Result(ErrorT&& err) noexcept;

    constexpr explicit Result(ValueT&& val) noexcept;

    NODISCARD_
    Result _clone_impl() const& noexcept override;


  public:
    constexpr Result() noexcept = default;

    constexpr explicit Result(Result&&) noexcept;

    static Result ok(ValueT&& val) noexcept;

    static Result err(ErrorT&& err) noexcept;

    NODISCARD_
    ValueT unwrap_or_default() && noexcept;

    NODISCARD_
    ErrorT error_or_default() && noexcept;

    NODISCARD_
    Bool is_ok() const& noexcept;

    NODISCARD_
    Bool is_err() const& noexcept;

    NODISCARD_
    OptionRef<const ValueT> as_ref() const& noexcept
    requires (not Reference<ValueT>);

    NODISCARD_
    OptionRef<ValueT> as_mut() & noexcept
    requires (not Reference<ValueT>);

    NODISCARD_
    Result& operator=(Result&&) noexcept;
};


// =============================================================================


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
constexpr Result<ValueT, ErrorT>::Result(ValueT&& val) noexcept:
    _value(do_move(val))
{ }


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
constexpr Result<ValueT, ErrorT>::Result(ErrorT&& err) noexcept:
    _is_err(true),
    _value(do_move(err))
{ }


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
constexpr Result<ValueT, ErrorT>::Result(Result&& obj) noexcept:
    _is_err(obj._is_err),
    _value(do_move(obj._value))
{ }


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
Result<ValueT, ErrorT> Result<ValueT, ErrorT>::ok(ValueT&& val) noexcept {
    return Result(do_move(val));
}


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
Result<ValueT, ErrorT> Result<ValueT, ErrorT>::err(ErrorT&& err) noexcept {
    return Result(do_move(err));
}


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
Result<ValueT, ErrorT> Result<ValueT, ErrorT>::_clone_impl() const& noexcept {
    if constexpr (!this->_is_err ? Clonable<ValueT> : Clonable<ErrorT>) {
        return Result((*reinterpret_cast<const ValueT*>(&this->_value)).clone());
    }
    else if constexpr (!this->_is_err ? Clonable<ValueT> : Clonable<ErrorT>) {
        return Result(do_move(*reinterpret_cast<const ValueT*>(&this->_value)));
    }
    else {
        return Result(); // TODO: Do abort
    }
}


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
ValueT Result<ValueT, ErrorT>::unwrap_or_default() && noexcept {
    static_assert(DefaultConstructible<ValueT>, "'ValueT' must be default constructible");
    return !this->_is_err ? do_move( *reinterpret_cast<ValueT*>(&this->_value)) : ValueT();
}


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
ErrorT Result<ValueT, ErrorT>::error_or_default() && noexcept {
    static_assert(DefaultConstructible<ErrorT>, "'ErrorT' must be default constructible");
    return !this->_is_err ? do_move( *reinterpret_cast<ErrorT*>(&this->_value)) : ErrorT();
}


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
Bool Result<ValueT, ErrorT>::is_ok() const& noexcept {
    return !this->_is_err;
}


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
Bool Result<ValueT, ErrorT>::is_err() const& noexcept {
    return this->_is_err;
}


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
OptionRef<const ValueT> Result<ValueT, ErrorT>::as_ref() const& noexcept
  requires (not Reference<ValueT>)
{
    return !this->_is_err ? OptionRef<const ValueT>::some(this->_value) : OptionRef<const ValueT>::none();
}


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
OptionRef<ValueT> Result<ValueT, ErrorT>::as_mut() & noexcept
  requires (not Reference<ValueT>)
{
    return !this->_is_err ? OptionRef<ValueT>::some(this->_value) : OptionRef<ValueT>::none();
}


template <typename ValueT, typename ErrorT>
  requires Movable<ValueT> and Movable<ErrorT>
Result<ValueT, ErrorT>& Result<ValueT, ErrorT>::operator=(Result&& obj) noexcept {
    this->_is_err = obj._is_err;
    this->_value = do_move(obj._value);
    return *this;
}



// =============================================================================