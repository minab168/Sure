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

        Void _placement_new(const T& val, Size idx) & noexcept requires (Clonable<T> or Copyable<T>) {
            if constexpr (Clonable<T>) {
                new (this->_slot_ptr(idx)) T(val.clone());
            }
            else {
                new (this->_slot_ptr(idx)) T(val);
            }
            this->_length += 1;
        }

        Void _placement_new(T&& val, Size idx) & noexcept requires MoveConstructible<T> {
            new (this->_slot_ptr(idx)) T(do_move(val));
            this->_length += 1;
        }

        template<typename ...Args, Size ...Idx>
        Void _placement_new(Args&&... args, IndexSeq<Idx...>) & noexcept {
            (new (this->_storage + Idx) T(do_forward<Args>(args)), ...);
            this->_length = sizeof...(Args);
        }

        T* _slot_ptr(Size idx) noexcept { return reinterpret_cast<T*>(this->_storage) + idx; }

      public:
        constexpr StaticVec() noexcept = default;

        template<typename ...Args>
            requires (sizeof...(Args) <= CAP)
        explicit StaticVec(Args&&... args) noexcept {
            this->_placement_new(do_forward<Args>(args)..., IndexSeq<sizeof...(Args)>{});
        }


        explicit StaticVec(StaticVec&& other) noexcept
            requires (MoveConstructible<T> or TriviallyMoveConstructible<T>)
        {
            if (other._length == 0) {
                return;
            }

            if constexpr (TriviallyMoveConstructible<T>) {
                mem_copy(this->_storage, other._storage, other._length * sizeof(T));
                this->_length = other._length;
            }
            else {
                for (mut i = 0; i < other._length; i++) {
                    T* src = other._slot_ptr(i);
                    new (this->_slot_ptr(i)) T(do_move(*src));
                }
            }
            this->_length = other._length;
        }


        const T* data() const& noexcept { return this->_slot_ptr(); }

        T* data() & noexcept { return this->_storage; }

        Iterator<const T> begin() const& noexcept { return Iterator<T>(this->_slot_ptr(0)); }

        Iterator<const T> end() const& noexcept { return Iterator<T>(this->_slot_ptr(this->_length)); }

        Iterator<T> begin() & noexcept { return Iterator<T>(this->_storage); }

        Iterator<const T> end() & noexcept { return Iterator<T>(this->_storage + this->_length); }

        NODISCARD_ Size length() const noexcept { return this->_length; }

        NODISCARD_ constexpr Size capacity() const noexcept { return CAP; }

        NODISCARD_ StaticVec clone() const& noexcept
            requires (Clonable<T> or Copyable<T>);

        T& operator[](Size idx) & noexcept;

        const T& operator[](Size idx) const& noexcept;

        T at_clone(Size idx) & noexcept;

        OptionRef<T> get_ref(Size idx) & noexcept;

        OptionRef<const T> get_ref(Size idx) const& noexcept;

        Option<T> get_clone(Size idx) const noexcept
        requires Clonable<T> or Copyable<T>;

        Void push(const T& value) & noexcept requires (Clonable<T> or Copyable<T>) {
            CHECK_ABORT(this->_length >= CAP, "Capacity exceeded!");
            this->_placement_new(value, this->_length);
        }

        Void push(T&& value) & noexcept requires MoveConstructible<T> {
            CHECK_ABORT(this->_length >= CAP, "Capacity exceeded!");
            this->_placement_new(do_move(value), this->_length);
        }

        NODISCARD_ Bool try_push(const T& value) & noexcept requires (Clonable<T> or Copyable<T>) { // TODO: Convert return type to `Result`
            if (this->_length >= CAP) return false;
            this->_placement_new(value, this->_length);
            return true;
        }

        NODISCARD_ Bool try_push(T&& value) & noexcept requires MoveConstructible<T> { // TODO: Convert return type to `Result`
            if (this->_length >= CAP) return false;
            this->_placement_new(value, this->_length);
            return true;
        }

        NODISCARD_ T pop() & noexcept requires MoveConstructible<T> {
            CHECK_ABORT(this->_length == 0, "Storage is empty!");

            this->_length -= 1;
            let slot = this->_slot_ptr(this->_length);
            mut result = do_move(*slot);
            slot->~T();

            return result;
        }

        NODISCARD_ Option<T> try_pop() & noexcept requires MoveConstructible<T> {
            if (this->_length == 0) return Option<T>::none();

            this->_length -= 1;
            let slot = this->_slot_ptr(this->_length);
            let result = do_move(*slot);
            slot->~T();

            return Option<T>::some(do_move(result));
        }

        StaticVec& operator=(StaticVec&& other) noexcept
            requires (MoveAssignable<T> or TriviallyMoveAssignable<T>)
        {
            if (this == &other) {
                return *this;
            }

            if (other._length == 0) {
                this->_length = 0;
                return *this;
            }

            if constexpr (TriviallyMoveConstructible<T>) {
                mem_copy(this->_storage, other._storage, other._length);
            }
            else {
                for (mut i = 0; i < this->_length; i++) {
                    this->_storage[i] = do_move(other._storage[i]);
                }
            }
            this->_length = other._length;
            return *this;
        }
    };

}


#endif // UTILS_COLLECTIONS_STATICS_STATIC_VEC_HPP
