#ifndef SURE_COMMON_TYPES_ARRAY_HPP
#define SURE_COMMON_TYPES_ARRAY_HPP


#include "../Ownership/Inc.hpp"
#include "../Ownership/Move.hpp"
#include "../Traits/Common.hpp"
#include "../Traits/Creation.hpp"
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
        Array _clone_impl(IndexSeq<Idx...>) const& noexcept;

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

        const T* data() const noexcept { return _data; }

        T* data() noexcept { return _data; }

        Iterator<const T> begin() const noexcept { return Iterator<const T>(this->_data); }

        Iterator<const T> end() const noexcept { return Iterator<const T>(this->_data + N); }

        Iterator<T> begin() noexcept { return Iterator<T>(this->_data); }

        Iterator<T> end() noexcept { return Iterator<T>(this->_data + N); }

        NODISCARD_ constexpr Size len() const& noexcept { return N; }

        NODISCARD_ Array clone() const& noexcept
            requires (Clonable<T> or CopyConstructible<T>);

        T& operator[](Size idx) & noexcept;

        const T& operator[](Size idx) const& noexcept;

        T at_clone(Size idx) const noexcept
            requires Clonable<T> or Copyable<T>;

        OptionRef<T> get_ref(Size idx) & noexcept;

        OptionRef<const T> get_ref(Size idx) const& noexcept;

        Option<T> get_clone(Size idx) const noexcept
            requires Clonable<T> or Copyable<T>;

        Array& operator=(Array&& other) noexcept
            requires MoveAssignable<T>;
    };

}


#include "Array.inl"


#endif // SURE_COMMON_TYPES_ARRAY_HPP