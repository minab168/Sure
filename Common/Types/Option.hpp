#pragma once


#include "Primitives/Inc.hpp"
#include "../Ownership/Inc.hpp"
#include "../Traits/Inc.hpp"
#include "../Utils/Abort.hpp"


template<typename RefT>
  requires (not Reference<RefT>)
class OptionRef final: NoDefaultCopy {
    RefT _ptr = NULL_PTR;

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
    RefT& value_or_abort() && noexcept;

    OptionRef& operator=(OptionRef&&) noexcept;

    constexpr ~OptionRef() noexcept override;
};


template<typename ValueT>
  requires (
      MoveConstructible<ValueT> and
      Destructible<ValueT> and
      NoThrowMovable<ValueT>
  )
class Option final: NoDefaultCopy /*, public Clone<Option<ValueT>> */ {

    static_assert(MoveConstructible<ValueT>, "Option<ValueT> requires ValueT to be movable object!");
    static_assert(Destructible<ValueT>, "Option<ValueT> requires ValueT to be destructible object!");
    static_assert(NoThrowMovable<ValueT>, "Option<ValueT> requires ValueT to not throw any exception in any operation!");

    Bool _has_value = false;

    // _storage: raw memory for ValueT; construct with placement-new, destroy only if Option lvalue survives.
    alignas(ValueT) U8 _storage[sizeof(ValueT)]{};

    constexpr explicit Option(ValueT&& val) noexcept;

    // Option _clone_impl() const& noexcept override;

    ValueT* _storage_ptr() noexcept;

    const ValueT* _storage_ptr() const noexcept;

    Void _destroy_value() noexcept;

  public:
    constexpr Option() noexcept = default;

    constexpr Option(Option&&) noexcept;

    static Option some(ValueT&& val) noexcept;

    static Option none() noexcept;

    NODISCARD_
    Bool is_some() const& noexcept;

    NODISCARD_
    Bool is_none() const& noexcept;

    NODISCARD_
    ValueT value_or_abort() && noexcept;

    NODISCARD_
    Option take() & noexcept;

    NODISCARD_
    Option clone() const& noexcept;

    NODISCARD_
    OptionRef<const ValueT> as_ref() const& noexcept
    requires (not Reference<ValueT>);

    NODISCARD_
    OptionRef<ValueT> as_mut() & noexcept
    requires (not Reference<ValueT>);

    Option& operator=(Option&&) noexcept
      requires (MoveAssignable<ValueT> /* and NoThrowMoveAssignable<ValueT>*/ );

    Option& operator=(Option&&) noexcept
      requires (not MoveAssignable<ValueT>);

    constexpr ~Option() noexcept override;
};


// ========================================================================


template<typename RefT>
  requires (not Reference<RefT>)
constexpr OptionRef<RefT>::OptionRef(RefT& ref) noexcept:
    _ptr(&ref)
{ }


template<typename RefT>
  requires (not Reference<RefT>)
constexpr OptionRef<RefT>::OptionRef(OptionRef&& obj) noexcept:
    _ptr(obj._ptr)
{
    obj._ptr = NULL_PTR;
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
    return this->_ptr != NULL_PTR;
}


template<typename RefT>
  requires (not Reference<RefT>)
Bool OptionRef<RefT>::is_none() const& noexcept {
    return !this->is_some();
}


template<typename RefT>
  requires (not Reference<RefT>)
RefT& OptionRef<RefT>::value_or_abort() && noexcept {
    if (this->is_none()) {
        abort_("No value is provided with the option!");
    }

    // No need to clear states. method takes ownership of this
    return *this->_ptr;
}


template<typename RefT>
  requires (not Reference<RefT>)
OptionRef<RefT>& OptionRef<RefT>::operator=(OptionRef&& obj) noexcept {
    if (this == &obj) return *this;

    this->_ptr = obj._ptr;
    obj._ptr   = NULL_PTR;
    return *this;
}


template<typename RefT>
  requires (!Reference<RefT>)
constexpr OptionRef<RefT>::~OptionRef() noexcept {
    this->_ptr = NULL_PTR;
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
Option<ValueT> Option<ValueT>::some(ValueT&& val) noexcept {
    return Option(do_move(val));
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
Option<ValueT> Option<ValueT>::clone() const& noexcept {
    static_assert(
        Clonable<ValueT> || Copyable<ValueT>,
        "Type of value in the option isn't `Copyable` or doesn't implement `Clonable`!"
    );

    if constexpr (Clonable<ValueT>) {
        return this->_has_value ? Option(this->_storage_ptr()->clone()) : Option();
    }
    else {
        return this->_has_value ? Option(*this->_storage_ptr()) : Option();
    }
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
constexpr Option<ValueT>::Option(Option&& obj) noexcept {
    if (obj._has_value) {
        new (this->_storage_ptr()) ValueT(do_move(*obj._storage_ptr()));
        this->_has_value = true;
        obj._destroy_value();
    }
    else {
        this->_has_value = false;
    }
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
constexpr Option<ValueT>::Option(ValueT&& val) noexcept {
    new (this->_storage_ptr()) ValueT(do_move(val));
    this->_has_value = true;
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
ValueT* Option<ValueT>::_storage_ptr() noexcept {
    return reinterpret_cast<ValueT*>(&this->_storage);
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
const ValueT* Option<ValueT>::_storage_ptr() const noexcept {
    return reinterpret_cast<const ValueT*>(&this->_storage);
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
Void Option<ValueT>::_destroy_value() noexcept {
    if (_has_value) {
        this->_storage_ptr()->~ValueT();
        _has_value = false;
    }
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
Option<ValueT> Option<ValueT>::none() noexcept {
    return Option();
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
Bool Option<ValueT>::is_some() const& noexcept {
    return this->_has_value;
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
Bool Option<ValueT>::is_none() const& noexcept {
    return !this->_has_value;
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
ValueT Option<ValueT>::value_or_abort() && noexcept {
    if (!this->_has_value) {
        abort_("No value is provided with the option!");
    }

    // No need to clear states. method takes ownership of this
    return do_move(*this->_storage_ptr());
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
Option<ValueT> Option<ValueT>::take() & noexcept {
    if (!this->_has_value) return {};

    let ret_res = Option::some(do_move(*this->_storage_ptr()));
    this->_destroy_value();
    return do_move(ret_res);
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
OptionRef<const ValueT> Option<ValueT>::as_ref() const& noexcept
  requires (not Reference<ValueT>)
{
    return this->_has_value ? OptionRef<const ValueT>::some(*this->_storage_ptr()) : OptionRef<const ValueT>::none();
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
OptionRef<ValueT> Option<ValueT>::as_mut() & noexcept
  requires (not Reference<ValueT>)
{
    return this->_has_value ? OptionRef<ValueT>::some(*this->_storage_ptr()) : OptionRef<ValueT>::none();
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
Option<ValueT>& Option<ValueT>::operator=(Option&& obj) noexcept
  requires (MoveAssignable<ValueT> /* and NoThrowMoveAssignable<ValueT> */)
{
    if (this == &obj) return *this;

    if (this->_has_value) {
        if (obj._has_value) {
            *this->_storage_ptr() = do_move(*obj._storage_ptr());
            obj._destroy_value();
        }
        else {
            this->_destroy_value();
        }
    }
    else {
        if (obj._has_value) {
            new (this->_storage_ptr()) ValueT(do_move(*obj._storage_ptr()));
            this->_has_value = true;
            obj._destroy_value();
        }
    }

    return *this;
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
Option<ValueT>& Option<ValueT>::operator=(Option&& obj) noexcept
  requires (not MoveAssignable<ValueT>)
{
    if (this == &obj) return *this;

    this->_destroy_value();

    if (obj._has_value) {
        new (this->_storage_ptr()) ValueT(do_move(*obj._storage_ptr()));
        this->_has_value = true;
        obj._destroy_value();
    }

    return *this;
}


template<typename ValueT>
  requires (MoveConstructible<ValueT> and Destructible<ValueT> and NoThrowMovable<ValueT>)
constexpr Option<ValueT>::~Option() noexcept {
    this->_destroy_value();
}



// ========================================================================


#define some_ref_(val_)         OptionRef<decltype(val_)>::some(val_)
#define some_ref_ty_(val_, ty_) OptionRef<ty_>::some(val_)
#define none_ref_(ty_)          OptionRef<ty_>::none()


#define some_(val_)         Option<decltype(val_)>::some(val_)
#define some_ty_(val_, ty_) Option<ty_>::some(val_)
#define none_(ty_)          Option<ty_>::none()