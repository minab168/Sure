/**
 * ============================================================================
 * @file       RingBuf
 * @brief      ${DESCRIPTION}
 * 
 * Part of the My Project
 * 
 * @author     emad
 * @date       2026-10-04
 * ============================================================================
*/


#ifndef UTILS_COLLECTIONS_STATICS_RING_BUF_HPP
#define UTILS_COLLECTIONS_STATICS_RING_BUF_HPP

#include "../../../Ownership/Inc.hpp"
#include "../../../Traits/Common.hpp"
#include "../../../Traits/Creation.hpp"
#include "../../../Types/Option.hpp"
#include "../../../Types/Primitives/Inc.hpp"


namespace sure::collections {

    template<typename T, Size CAP>
    class RingBuf final: NoDefaultCopy {

        alignas(T) U8 _storage[sizeof(T) * CAP];
        U8 _head = 0;
        U8 _tail = 0;

        Void _advanceTailIfFull() noexcept;

        Void _placementNew(const T& value, Size idx) noexcept requires (Clonable<T> or Copyable<T>);

        Void _placementNew(T&& value, Size idx) noexcept;

        T* _getStoragePtr(Size idx) & noexcept { return reinterpret_cast<T*>(this->_storage) + idx % CAP; }

        const T* _getStoragePtr(Size idx) const& noexcept { return reinterpret_cast<const T*>(this->_storage) + idx % CAP; }

      public:
        constexpr RingBuf() noexcept = default;

        RingBuf(RingBuf&&) noexcept requires (MoveConstructible<T> or TriviallyMoveConstructible<T>);

        static RingBuf new_zeroed() noexcept;

        Size len() noexcept { return this->_head - this->_tail; }

        NODISCARD_ Bool isEmpty() const noexcept { return this->_head == this->_tail; }

        NODISCARD_ Bool isFull() const noexcept { return this->_head - this->_tail == CAP; }

        RingBuf clone() const& noexcept requires (Clonable<T> or Copyable<T>);

        Void push(const T& obj) & noexcept requires (Copyable<T> or Clonable<T>);

        Void push(T&& obj) & noexcept;

        T pop() & noexcept;

        Option<T> tryPop() & noexcept;

        Void clear() & noexcept;

        Void reset() && noexcept;


        RingBuf& operator=(RingBuf&&) noexcept requires (MoveAssignable<T> or TriviallyMoveAssignable<T>);
    };

}

#include "RingBuf.inl"

#endif // UTILS_COLLECTIONS_STATICS_RING_BUF_HPP

