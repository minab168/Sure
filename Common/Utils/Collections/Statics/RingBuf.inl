#ifndef UTILS_COLLECTIONS_STATICS_RING_BUF_INL
#define UTILS_COLLECTIONS_STATICS_RING_BUF_INL

#ifndef UTILS_COLLECTIONS_STATICS_RING_BUF_HPP
    #error "Do not include this file directly"
#endif // UTILS_COLLECTIONS_STATICS_RING_BUF_HPP

#include "../../Check.hpp"
#include "../../Memory.hpp"
#include "../../../Base/Interface/Lang.hpp"
#include "../../../Traits/Common.hpp"
#include "../../../Traits/Creation.hpp"


namespace sure::collections {

    template<typename T, Size CAP>
    Void RingBuf<T, CAP>::_advanceTailIfFull() noexcept {
        if (this->isFull()) {
            mut last = this->_getStoragePtr(this->_tail);
            last->~T();
            this->_tail += 1;
        }
    }


    template<typename T, Size CAP>
    Void RingBuf<T, CAP>::_placementNew(const T& value, const Size idx) noexcept requires (Clonable<T> or Copyable<T>) {
        if constexpr (Clonable<T>) {
            new (this->_getStoragePtr(idx % CAP)) T{value.clone()};
        }
        else {
            new (this->_getStoragePtr(idx % CAP)) T{value};
        }
    }


    template<typename T, Size CAP>
    Void RingBuf<T, CAP>::_placementNew(T&& value, const Size idx) noexcept {
        new (this->_getStoragePtr(idx)) T{do_forward<T>(value)};
    }

    template<typename T, Size CAP>
    RingBuf<T, CAP> RingBuf<T, CAP>::new_zeroed() noexcept {
        // TODO
    }


    template<typename T, Size CAP>
    RingBuf<T, CAP>::RingBuf(RingBuf&& other) noexcept
        requires (MoveConstructible<T> or TriviallyMoveConstructible<T>)
    {
        if (other.isEmpty()) {
            return;
        }

        if constexpr (TriviallyMoveConstructible<T>) {
            mem_copy(this->_storage, other._storage, this->len() * sizeof(T));
        }
        else {
            for (mut i = 0; i < this->len(); i++) {
                let idx = this->_tail + i;
                let src = other._getStoragePtr(idx);
                this->_placementNew(do_move(src));
                src->~T();
            }
        }

        this->_head = other._head;
        this->_tail = other._tail;
        other._head = 0;
        other._tail = 0;
    }




    template<typename T, Size CAP>
    RingBuf<T, CAP> RingBuf<T, CAP>::clone() const & noexcept requires (Clonable<T> or Copyable<T>) {
        mut new_obj = RingBuf();

        for (mut i = 0; i < this->len(); i++) {
            let idx = this->_tail + i;
            let src = this->_getStoragePtr(idx);
            new_obj._placementNew(*src, idx);
        }

        new_obj._head = this->_head;
        new_obj._tail = this->_tail;
        return new_obj;
    }


    template<typename T, Size CAP>
    Void RingBuf<T, CAP>::push(const T& obj) & noexcept requires (Copyable<T> or Clonable<T>) {
        this->_placementNew(obj, this->_head);
        this->_head += 1;

        this->_advanceTailIfFull();
    }


    template<typename T, Size CAP>
    Void RingBuf<T, CAP>::push(T&& obj) & noexcept {
        this->_placementNew(obj, this->_head);
        this->_head += 1;

        this->_advanceTailIfFull();
    }

    template<typename T, Size CAP>
    T RingBuf<T, CAP>::pop() & noexcept {
        ABORT_IF_FALSE_(!this->isEmpty(), "Cannot pop: ring buffer is empty!");

        mut obj = this->_getStoragePtr(this->_tail);
        mut ret = do_move(*obj);
        obj->~T();
        this->_tail += 1;
        return ret;
    }

    template<typename T, Size CAP>
    Option<T> RingBuf<T, CAP>::tryPop() & noexcept {
        if (this->isEmpty()) {
            return Option<T>::none();
        }

        mut obj = this->_getStoragePtr(this->_tail);
        mut ret = do_move(*obj);
        obj->~T();
        this->_tail += 1;
        return Option<T>::some(do_move(ret));
    }


    template<typename T, Size CAP>
    Void RingBuf<T, CAP>::clear() & noexcept {
        for (mut i = 0; i < this->len(); i++) {
            let idx = this->_tail + i;
            let src = this->_getStoragePtr(idx);
            src->~T();
        }
        this->_head = this->_tail;
    }


    template<typename T, Size CAP>
    Void RingBuf<T, CAP>::reset() && noexcept {
        this->clear();
        this->_head = 0;
        this->_tail = 0;
    }


    template<typename T, Size CAP>
    RingBuf<T, CAP>& RingBuf<T, CAP>::operator=(RingBuf&&) noexcept
        requires (MoveAssignable<T> or TriviallyMoveAssignable<T>)
    {
        // TODO
    }
}

#endif // UTILS_COLLECTIONS_STATICS_RING_BUF_INL