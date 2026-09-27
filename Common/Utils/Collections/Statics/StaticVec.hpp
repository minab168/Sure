/**
 * ============================================================================
 * @file       StaticVec
 * @brief      ${DESCRIPTION}
 *
 * Part of the My Project
 *
 * @author     emad
 * @date       2026-09-17
 * ============================================================================
 */


#ifndef UTILS_COLLECTIONS_STATICS_STATIC_VEC_HPP
#define UTILS_COLLECTIONS_STATICS_STATIC_VEC_HPP

#include "../../Memory.hpp"
#include "../../../Types/Array.hpp"
#include "../../../Types/Result.hpp"
#include "../../../Types/Primitives/Inc.hpp"


namespace sure::collections {

    template<typename T, Size CAP>
    class StaticVec: NoDefaultCopy {

        alignas(T) U8 _storage[CAP * sizeof(T)]{};
        Size          _length = 0;

        Void _placement_new(const T& val, Size idx) & noexcept requires (Clonable<T> or Copyable<T>);

        Void _placement_new(T&& val, Size idx) & noexcept requires MoveConstructible<T>;

        template<typename ...Args, Size ...Idx>
        Void _placement_new(IndexSeq<Idx...>, Args&&... args) & noexcept;

        StaticVec _impl_clone() const& noexcept requires (Clonable<T> or Copyable<T>);

        T* _slot_ptr(Size idx) & noexcept { return reinterpret_cast<T*>(this->_storage) + idx; }

        const T* _slot_ptr(Size idx) const& noexcept { return reinterpret_cast<const T*>(this->_storage) + idx; }

      public:
        constexpr StaticVec() noexcept = default;

        ~StaticVec() noexcept;

        template<typename ...Args>
            requires (sizeof...(Args) <= CAP)
        explicit StaticVec(Args&&... args) noexcept;

        StaticVec(StaticVec&& other) noexcept requires (MoveConstructible<T> or TriviallyMoveConstructible<T>);


        NODISCARD_ const T* data() const& noexcept { return this->_slot_ptr(0); }

        NODISCARD_ T* data() & noexcept { return this->_slot_ptr(0); }

        Iterator<const T> begin() const& noexcept { return Iterator<const T>(this->_slot_ptr(0)); }

        Iterator<const T> end() const& noexcept { return Iterator<const T>(this->_slot_ptr(this->_length)); }

        Iterator<T> begin() & noexcept { return Iterator<T>(this->_slot_ptr(0)); }

        Iterator<T> end() & noexcept { return Iterator<T>(this->_slot_ptr(this->_length)); }

        NODISCARD_ Size length() const noexcept { return this->_length; }

        NODISCARD_ consteval Size capacity() const noexcept { return CAP; }

        NODISCARD_ StaticVec clone() const& noexcept requires (Clonable<T> or Copyable<T>);

        T& operator[](Size idx) & noexcept;

        const T& operator[](Size idx) const& noexcept;

        T at_clone(Size idx) & noexcept requires Clonable<T> or Copyable<T>;

        OptionRef<T> get_ref(Size idx) & noexcept;

        OptionRef<const T> get_ref(Size idx) const& noexcept;

        Option<T> get_clone(Size idx) const noexcept requires Clonable<T> or Copyable<T>;

        Void push(const T& value) & noexcept requires (Clonable<T> or Copyable<T>);

        Void push(T&& value) & noexcept requires MoveConstructible<T>;

        NODISCARD_ Bool try_push(const T& value) & noexcept requires (Clonable<T> or Copyable<T>);

        NODISCARD_ Bool try_push(T&& value) & noexcept requires MoveConstructible<T>;

        NODISCARD_ T pop() & noexcept requires MoveConstructible<T>;

        NODISCARD_ Option<T> try_pop() & noexcept requires MoveConstructible<T>;

        StaticVec& operator=(StaticVec&& other) noexcept requires (MoveAssignable<T> or TriviallyMoveAssignable<T>);
    };

}


#include "StaticVec.inl"


#endif // UTILS_COLLECTIONS_STATICS_STATIC_VEC_HPP
