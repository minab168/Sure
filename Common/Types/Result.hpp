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


#include "Option.hpp"
#include "../Ownership/Inc.hpp"
#include "../Traits/Common.hpp"
#include "../Traits/Creation.hpp"
#include "Primitives/Inc.hpp"


#define RESULT_CLASS_TEMPLATE_CONSTRAINT \
  requires Movable<ValueT> and Movable<ErrorT> and \
    Destructible<ValueT> and Destructible<ErrorT> and \
NoThrowMovable<ValueT> and NoThrowMovable<ErrorT>


template<typename VRefT, typename ERefT>
  requires (not Reference<VRefT> and not Reference<ERefT>)
class ResultRef final: NoDefaultCopy {
    VRefT _vptr = NULL_PTR;
    ERefT _eptr = NULL_PTR;

    constexpr explicit ResultRef(VRefT& vref) noexcept;

    constexpr explicit ResultRef(ERefT& eref) noexcept;

  public:
    constexpr ResultRef() noexcept = default;

    constexpr ResultRef(ResultRef&&) noexcept;

    static ResultRef ok(VRefT& vref) noexcept;

    static ResultRef err(ERefT& eref) noexcept;

    NODISCARD_
    VRefT unwrap() && noexcept;

    NODISCARD_
    OptionRef<VRefT> value() && noexcept;

    NODISCARD_
    OptionRef<ERefT> error() && noexcept;

    NODISCARD_
    Bool is_ok() const& noexcept;

    NODISCARD_
    Bool is_err() const& noexcept;

    ResultRef& operator=(ResultRef&&) noexcept;

    constexpr ~ResultRef() noexcept override;
};


template<typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
class Result final: NoDefaultCopy {
    Bool               _is_err = false;
    alignas(ValueT) U8 _value_storage[sizeof(ValueT)]{};
    alignas(ErrorT) U8 _error_storage[sizeof(ErrorT)]{};

    constexpr explicit Result(ErrorT&& err) noexcept;

    constexpr explicit Result(ValueT&& val) noexcept;

    NODISCARD_
    ValueT* _value_storage_ptr() noexcept;

    NODISCARD_
    const ValueT* _value_storage_ptr() const& noexcept;

    NODISCARD_
    ErrorT* _error_storage_ptr() noexcept;

    NODISCARD_
    const ErrorT* _error_storage_ptr() const& noexcept;

    constexpr Void _destroy() noexcept;


  public:
    constexpr Result() noexcept = default;

    constexpr explicit Result(Result&&) noexcept;

    static Result ok(ValueT&& val) noexcept;

    static Result err(ErrorT&& err) noexcept;

    NODISCARD_
    ValueT unwrap() && noexcept
      requires Printable<ErrorT>;

    NODISCARD_
    ValueT expect(const Char* msg) && noexcept
      requires Printable<ErrorT>;

    NODISCARD_
    ErrorT unwrap_err() && noexcept
      requires Printable<ValueT>;

    NODISCARD_
    Option<ValueT> value() && noexcept;

    NODISCARD_
    Option<ErrorT> error() && noexcept;

    NODISCARD_
    Bool is_ok() const& noexcept;

    NODISCARD_
    Bool is_err() const& noexcept;

    NODISCARD_
    ResultRef<const ValueT, const ErrorT> as_ref() const& noexcept
    requires (not Reference<ValueT>);

    NODISCARD_
    ResultRef<ValueT, ErrorT> as_mut() & noexcept
    requires (not Reference<ValueT>);

    NODISCARD_
    Result clone() const& noexcept;

    Result& operator=(Result&&) noexcept
      requires MoveAssignable<ValueT> and MoveAssignable<ErrorT>;

    Result& operator=(Result&&) noexcept
      requires (not (MoveAssignable<ValueT> and MoveAssignable<ErrorT>));

    constexpr ~Result() noexcept override;
};


// =============================================================================


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
constexpr ResultRef<VRefT, ERefT>::ResultRef(VRefT& vref) noexcept:
    _vptr(&vref),
    _eptr(NULL_PTR)
{ }


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
constexpr ResultRef<VRefT, ERefT>::ResultRef(ERefT& eref) noexcept:
    _vptr(NULL_PTR),
    _eptr(&eref)
{ }


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
constexpr ResultRef<VRefT, ERefT>::ResultRef(ResultRef&& obj) noexcept:
    _vptr(obj._vptr),
    _eptr(obj._eptr)
{
    obj._vptr = NULL_PTR;
    obj._eptr = NULL_PTR;
}


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
ResultRef<VRefT, ERefT> ResultRef<VRefT, ERefT>::ok(VRefT& vref) noexcept {
    return ResultRef(vref);
}


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
ResultRef<VRefT, ERefT> ResultRef<VRefT, ERefT>::err(ERefT& eref) noexcept {
    return ResultRef(eref);
}


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
Bool ResultRef<VRefT, ERefT>::is_ok() const& noexcept {
    return this->_vptr != NULL_PTR;
}


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
Bool ResultRef<VRefT, ERefT>::is_err() const& noexcept {
    return this->_eptr != NULL_PTR;
}


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
VRefT ResultRef<VRefT, ERefT>::unwrap() && noexcept {
    if (this->is_err()) {
        abort_("Result contains error. cannot be unwrapped!");
    }
    return *this->_vptr;
}


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
OptionRef<VRefT> ResultRef<VRefT, ERefT>::value() && noexcept {
    return this->is_ok() ? some_ref_(*this->_vptr) : none_ref_(VRefT);
}


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
OptionRef<ERefT> ResultRef<VRefT, ERefT>::error() && noexcept {
    return this->is_err() ? some_ref_(*this->_eptr) : none_ref_(ERefT);
}


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
ResultRef<VRefT, ERefT>& ResultRef<VRefT, ERefT>::operator=(ResultRef&& obj) noexcept {
    if (this == &obj) return *this;

    this->_vptr = obj._vptr;
    this->_eptr = obj._eptr;

    obj._vptr = NULL_PTR;
    obj._eptr = NULL_PTR;
    return *this;
}


template<typename VRefT, typename ERefT>
  requires (!Reference<VRefT> && !Reference<ERefT>)
constexpr ResultRef<VRefT, ERefT>::~ResultRef() noexcept {
    this->_vptr = NULL_PTR;
    this->_eptr = NULL_PTR;
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
constexpr Result<ValueT, ErrorT>::Result(ValueT&& val) noexcept {
    new (this->_value_storage_ptr()) ValueT(do_move(val));
    this->_is_err = false;
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
constexpr Result<ValueT, ErrorT>::Result(ErrorT&& err) noexcept {
    new (this->_error_storage_ptr()) ErrorT(do_move(err));
    this->_is_err = true;
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
ValueT* Result<ValueT, ErrorT>::_value_storage_ptr() noexcept {
    return reinterpret_cast<ValueT*>(&this->_value_storage);
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
const ValueT* Result<ValueT, ErrorT>::_value_storage_ptr() const& noexcept {
    return reinterpret_cast<const ValueT*>(&this->_value_storage);
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
ErrorT* Result<ValueT, ErrorT>::_error_storage_ptr() noexcept {
    return reinterpret_cast<ErrorT*>(&this->_error_storage);
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
const ErrorT* Result<ValueT, ErrorT>::_error_storage_ptr() const& noexcept {
    return reinterpret_cast<const ErrorT*>(&this->_error_storage);
}


template<typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
constexpr Void Result<ValueT, ErrorT>::_destroy() noexcept {
    if (this->is_ok()) {
        this->_value_storage_ptr()->~ValueT();
    }
    else {
        this->_error_storage_ptr()->~ErrorT();
    }
    this->_is_err = false;
}



template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
constexpr Result<ValueT, ErrorT>::Result(Result&& obj) noexcept {
    if (obj.is_ok()) {
        new (this->_value_storage_ptr()) ValueT(do_move(*obj._value_storage_ptr()));
        this->_is_err = false;
    }
    else {
        this->_is_err = true;
        new (this->_error_storage_ptr()) ErrorT(do_move(*obj._error_storage_ptr()));
    }
    obj._destroy();
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
Result<ValueT, ErrorT> Result<ValueT, ErrorT>::ok(ValueT&& val) noexcept {
    return Result(do_move(val));
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
Result<ValueT, ErrorT> Result<ValueT, ErrorT>::err(ErrorT&& err) noexcept {
    return Result(do_move(err));
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
Result<ValueT, ErrorT> Result<ValueT, ErrorT>::clone() const& noexcept {
    if (this->is_ok()) {
        if constexpr (Clonable<ValueT>)
            return Result(this->_value_storage_ptr()->clone());
        else
            return Result(*this->_value_storage_ptr());
    }

    if constexpr (Clonable<ErrorT>)
        return Result(this->_error_storage_ptr()->clone());
    else
        return Result(*this->_error_storage_ptr());
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
ValueT Result<ValueT, ErrorT>::unwrap() && noexcept
  requires Printable<ErrorT>
{
    if (this->is_err()) {
        if constexpr (Primitive<ValueT>) {
            abort_("Result contains error. cannot be unwrapped for value | %d", *this->_error_storage_ptr()); // TODO: Fix it to cover all possible primitive types
        }
        else {
            abort_("Result contains error. cannot be unwrapped for value | %s", this->_error_storage_ptr()->print());
        }
    }
    return do_move( *this->_value_storage_ptr());
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
ValueT Result<ValueT, ErrorT>::expect(const Char* msg) && noexcept
  requires Printable<ErrorT>
{
    if (this->is_err()) {
        abort_("%s | %s", msg, this->_error_storage_ptr()->print());
    }
    return do_move( *this->_value_storage_ptr());
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
ErrorT Result<ValueT, ErrorT>::unwrap_err() && noexcept
  requires Printable<ValueT>
{
    if (this->is_ok()) {
        if constexpr (Primitive<ValueT>) {
            abort_("Result contains value. cannot be unwrapped for error | %d", *this->_value_storage_ptr()); // TODO: Fix it to cover all possible primitive types
        }
        else {
            abort_("Result contains value. cannot be unwrapped for error | %s", this->_value_storage_ptr()->print());
        }
    }
    return do_move( *this->_error_storage_ptr());
}


template<typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
Option<ValueT> Result<ValueT, ErrorT>::value() && noexcept {
    return this->is_ok() ? some_(do_move(*this->_value_storage_ptr())) : none_(ValueT);
}


template<typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
Option<ErrorT> Result<ValueT, ErrorT>::error() && noexcept {
    return this->is_err() ? some_(do_move(*this->_error_storage_ptr())) : none_(ErrorT);
}



template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
Bool Result<ValueT, ErrorT>::is_ok() const& noexcept {
    return !this->_is_err;
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
Bool Result<ValueT, ErrorT>::is_err() const& noexcept {
    return this->_is_err;
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
ResultRef<const ValueT, const ErrorT> Result<ValueT, ErrorT>::as_ref() const& noexcept
  requires (not Reference<ValueT>)
{
    return !this->_is_err ?
        ResultRef<const ValueT, const ErrorT>::ok(*this->_value_storage_ptr())
    :
        ResultRef<const ValueT, const ErrorT>::err(*this->_error_storage_ptr());
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
ResultRef<ValueT, ErrorT> Result<ValueT, ErrorT>::as_mut() & noexcept
  requires (not Reference<ValueT>)
{
    return !this->_is_err ?
        ResultRef<ValueT, ErrorT>::ok(*this->_value_storage_ptr())
    :
        ResultRef<ValueT, ErrorT>::err(*this->_error_storage_ptr());
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
Result<ValueT, ErrorT>& Result<ValueT, ErrorT>::operator=(Result&& obj) noexcept
  requires MoveAssignable<ValueT> and MoveAssignable<ErrorT>
{
    if (this == &obj) return *this;

    if (this->is_ok()) {
        if (obj.is_ok()) {
            *this->_value_storage_ptr() = do_move(*obj._value_storage_ptr());
        } else {
            this->_value_storage_ptr()->~ValueT();
            new (this->_error_storage_ptr()) ErrorT(do_move(*obj._error_storage_ptr()));
            this->_is_err = true;
        }
    } else {
        if (obj.is_ok()) {
            this->_error_storage_ptr()->~ErrorT();
            new (this->_value_storage_ptr()) ValueT(do_move(*obj._value_storage_ptr()));
            this->_is_err = false;
        } else {
            *this->_error_storage_ptr() = do_move(*obj._error_storage_ptr());
        }
    }
    obj._destroy();
    return *this;
}


template <typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
Result<ValueT, ErrorT>& Result<ValueT, ErrorT>::operator=(Result&& obj) noexcept
  requires (not (MoveAssignable<ValueT> and MoveAssignable<ErrorT>))
{
    if (this == &obj) return *this;

    if (this->is_ok()) {
        if (obj.is_ok()) {
            this->_value_storage_ptr()->~ValueT();
            new (this->_value_storage_ptr()) ValueT(do_move(*obj._value_storage_ptr()));
            this->_is_err = false;
        } else {
            new (this->_error_storage_ptr()) ErrorT(do_move(*obj._error_storage_ptr()));
            this->_value_storage_ptr()->~ValueT();
            this->_is_err = true;
        }
    } else {
        if (obj.is_ok()) {
            new (this->_value_storage_ptr()) ValueT(do_move(*obj._value_storage_ptr()));
            this->_error_storage_ptr()->~ErrorT();
            this->_is_err = false;
        } else {
            this->_error_storage_ptr()->~ErrorT();
            new (this->_error_storage_ptr()) ErrorT(do_move(*obj._error_storage_ptr()));
            this->_is_err = true;
        }
    }
    obj._destroy();
    return *this;
}


template<typename ValueT, typename ErrorT>
RESULT_CLASS_TEMPLATE_CONSTRAINT
constexpr Result<ValueT, ErrorT>::~Result() noexcept {
    this->_destroy();
}



// =============================================================================