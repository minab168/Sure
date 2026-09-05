//
// Created by user on 12/09/2026.
//

#ifndef SURE_COMMON_TYPES_ARRAY_HPP
#define SURE_COMMON_TYPES_ARRAY_HPP

#include <utility>

#include "../Ownership/Inc.hpp"
#include "../Ownership/Move.hpp"
#include "../Traits/Common.hpp"
#include "../Traits/Creation.hpp"
#include "../Utils/Check.hpp"
#include "Primitives/Inc.hpp"
#include "Option.hpp"
#include "IndexSeq.hpp"
#include "Iterator.hpp"


namespace sure {

    template<typename T, Size N>
    class Array: NoDefaultCopy { // Satisfies Clonable

        static_assert(N > 0, "Empty Array is not valid.");

        T _data[N];


        template<Size... Idx>
        requires Clonable<T> or CopyConstructible<T>
        Array _clone_impl(IndexSeq<Idx...>) const& noexcept {
            if constexpr (Clonable<T>) {
                return Array { this->_data[Idx].clone() ... };
            }
            else {
                return Array { this->_data[Idx] ... };
            }
        }


        template<Size... Idx>
        requires MoveAssignable<T>
        Void _move_assign_impl(Array&& other, IndexSeq<Idx...>) noexcept {
            ((this->_data[Idx] = do_move(other._data[Idx])), ... );
        }


        template<Size ...Idx>
            requires MoveConstructible<T>
        Array(Array&& other, IndexSeq<Idx...>) noexcept :
        _data{do_move(other._data[Idx])...}
        { }

    public:
        constexpr Array() noexcept
            requires DefaultConstructible<T> = default;

        template<typename ...Args>
            requires (sizeof...(Args) == N)
        constexpr explicit Array(Args&&...args) noexcept:
            _data{do_forward<Args>(args)...}
        { }


        constexpr Array(Array&& other) noexcept
            requires MoveConstructible<T>:
        Array(do_forward<Array&&>(other), MakeIdxSeq<N>{})
        { }


        NODISCARD_
        Array clone() const& noexcept requires (Clonable<T> or CopyConstructible<T>) {
            #if !SURE__HAS_BUILTIN_(__make_integer_seq)
                static_assert(
                    N <= 512,
                    "Array<T,N> with N > 512 needs compiler builtin support (__make_integer_seq)"
                    "for index-sequence generation, or a higher -ftemplate-depth if your compiler"
                    "supports the recursive fallback."
                );
            #endif
            return this->_clone_impl(MakeIdxSeq<N>{});
        }


        T& operator[](Size idx) & noexcept {
            CHECK_ABORT(idx >= N, "Array length exceeded.");
            return this->_data[idx];
        }


        const T& operator[](Size idx) const& noexcept {
            CHECK_ABORT(idx >= N, "Array length exceeded.");
            return this->_data[idx];
        }


        T at_clone(Size idx) const noexcept requires Clonable<T> or Copyable<T> {
            CHECK_ABORT(idx >= N, "Array length exceeded.");
            if constexpr (Clonable<T>) {
                return this->_data[idx].clone();
            }
            else {
                return this->_data[idx];
            }
        }


        OptionRef<T> get_ref(Size idx) & noexcept {
            return idx < N ? OptionRef<T>::some(this->_data[idx]) : OptionRef<T>::none();
        }


        OptionRef<const T> get_ref(Size idx) const& noexcept {
            return idx < N ? OptionRef<const T>::some(this->_data[idx]) : OptionRef<const T>::none();
        }


        Option<T> get_clone(Size idx) const noexcept requires Clonable<T> or Copyable<T> {
            if constexpr (Clonable<T>) {
                return idx < N ? Option<T>::some(this->_data[idx].clone()) : Option<T>::none();
            }
            else {
                return idx < N ? Option<T>::some(T(this->_data[idx])) : Option<T>::none();
            }
        }


        const T* data() const noexcept {
            return _data;
        }


        T* data() noexcept {
            return _data;
        }


        Iterator<const T> begin() const noexcept {
            return Iterator<const T>(this->_data);
        }


        Iterator<const T> end() const noexcept {
            return Iterator<const T>(this->_data + N);
        }


        Iterator<T> begin() noexcept {
            return Iterator<T>(this->_data);
        }


        Iterator<T> end() noexcept {
            return Iterator<T>(this->_data + N);
        }


        NODISCARD_
        constexpr Size len() const& noexcept {
            return N;
        }


        Array& operator=(Array&& other) noexcept
            requires MoveAssignable<T>
        {
            if (this != &other) {
                this->_move_assign_impl(do_forward<Array&&>(other), MakeIdxSeq<N>());
            }
            return *this;
        }
    };

}


#endif // SURE_COMMON_TYPES_ARRAY_HPP