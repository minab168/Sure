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

    /**
     * @brief Fixed-size contiguous array.
     *
     * @tparam T Element type.
     * @tparam N Number of elements. Must be greater than zero.
     *
     * @note Array does not provide copy construction or copy assignment.
     *       Elements are moved when move operations are available.
     *
     * @note Element access is unchecked. Use get_ref(), get_clone(), or
     *       at_clone() when an optional result is required.
     *
     * @remarks The array stores its elements directly inside the object.
     *          No dynamic allocation is performed.
     */
    template<typename T, Size N>
    class Array: NoDefaultCopy { /* Satisfies Clonable, Iterable */
        static_assert(N > 0, "Empty Array is not valid.");

        T _data[N];

        // Expands the index sequence to clone/copy all elements in one
        // aggregate initialization. Clonable types use clone(); otherwise
        // elements are copy-constructed.
        template<Size... Idx>
            requires Clonable<T> or CopyConstructible<T>
        Array _clone_impl(IndexSeq<Idx...>) const& noexcept;

        // Expands the index sequence so each element is move-assigned individually.
        template<Size... Idx>
            requires MoveAssignable<T>
        Void _move_assign_impl(Array&& other, IndexSeq<Idx...>) noexcept {
            ((this->_data[Idx] = do_move(other._data[Idx])), ... );
        }

        // Used by the public move constructor to expand the index sequence
        // and move-construct every element directly into the array.
        template<Size ...Idx>
            requires MoveConstructible<T>
        Array(Array&& other, IndexSeq<Idx...>) noexcept :
            _data{do_move(other._data[Idx])...}
        { }

    public:
        /**
         * @brief Default-constructs all elements.
         *
         * @requires T is DefaultConstructible.
         */
        constexpr Array() noexcept
            requires DefaultConstructible<T> = default;

        /**
         * @brief Constructs the array from exactly N arguments.
         *
         * Each argument is forwarded to the corresponding element.
         *
         * @param args Constructor arguments for the elements.
         *
         * @requires The number of arguments is exactly N.
         */
        template<typename ...Args>
            requires (sizeof...(Args) == N)
        constexpr explicit Array(Args&&...args) noexcept:
            _data{do_forward<Args>(args)...}
        { }

        /**
         * @brief Move-constructs the array.
         *
         * Elements are move-constructed from the @p other.
         *
         * @param other Source array.
         *
         * @requires T is MoveConstructible.
         */
        constexpr Array(Array&& other) noexcept
            requires MoveConstructible<T>:
            Array(do_forward<Array&&>(other), MakeIdxSeq<N>{})
        { }

        /**
         * @brief Returns a pointer to the underlying contiguous storage.
         *
         * @return Constant pointer to the first element.
         */
        const T* data() const noexcept { return _data; }

        /**
         * @brief Returns a mutable pointer to the underlying contiguous storage.
         *
         * @return Pointer to the first element.
         */
        T* data() noexcept { return _data; }

        /**
         * @brief Returns a const iterator to the first element.
         *
         * @return Iterator pointing to the first element.
         */
        Iterator<const T> begin() const noexcept { return Iterator<const T>(this->_data); }

        /**
         * @brief Returns a const iterator past the last element.
         *
         * @return Iterator pointing past the last element.
         */
        Iterator<const T> end() const noexcept { return Iterator<const T>(this->_data + N); }

        /**
         * @brief Returns a mutable iterator to the first element.
         *
         * @return Iterator pointing to the first element.
         */
        Iterator<T> begin() noexcept { return Iterator<T>(this->_data); }

        /**
         * @brief Returns a mutable iterator past the last element.
         *
         * @return Iterator pointing past the last element.
         */
        Iterator<T> end() noexcept { return Iterator<T>(this->_data + N); }

        /**
         * @brief Returns the number of elements.
         *
         * @return The compile-time array size N.
         */
        NODISCARD_ constexpr Size len() const& noexcept { return N; }

        /**
         * @brief Creates a cloned copy of the entire array.
         *
         * @note Large arrays may require compiler support for __make_integer_seq
         *       or a sufficiently high template instantiation depth.
         *
         * @requires T is Clonable or CopyConstructible.
         */
        NODISCARD_ Array clone() const& noexcept
            requires (Clonable<T> or CopyConstructible<T>);

        /**
         * @brief Accesses an element by index with runtime bounds checking.
         *
         * Aborts execution when idx is outside [0, N).
         *
         * @param idx Element index.
         * @return Reference to the selected element.
         */
        T& operator[](Size idx) & noexcept;

        /**
         * @brief Accesses an element by index with runtime bounds checking.
         *
         * Aborts execution when idx is outside [0, N).
         *
         * @param idx Element index.
         * @return Constant reference to the selected element.
         */
        const T& operator[](Size idx) const& noexcept;

        /**
         * @brief Clones or copies the element at the given index.
         *
         * Aborts execution when idx is outside [0, N).
         * Uses T::clone() when T is Clonable; otherwise performs copy construction.
         *
         * @param idx Element index.
         * @return Cloned or copied element.
         *
         * @requires T is Clonable or Copyable.
         */
        T at_clone(Size idx) const noexcept
            requires Clonable<T> or Copyable<T>;

        /**
         * @brief Returns an optional reference to the element at the given index.
         *
         * Returns none() when idx is outside [0, N); otherwise returns a reference
         * to the selected element.
         *
         * @param idx Element index.
         * @return OptionRef containing the element reference when the index is valid.
         */
        OptionRef<T> get_ref(Size idx) & noexcept;

        /**
         * @brief Returns an optional const reference to the element at the given index.
         *
         * Returns none() when idx is outside [0, N); otherwise returns a const reference
         * to the selected element.
         *
         * @param idx Element index.
         * @return OptionRef containing the element reference when the index is valid.
         */
        OptionRef<const T> get_ref(Size idx) const& noexcept;

        /**
         * @brief Clones or copies an element when the index is valid.
         *
         * Returns none() when idx is outside [0, N).
         * Uses T::clone() when T is Clonable; otherwise performs copy construction.
         *
         * @param idx Element index.
         * @return Option containing the cloned or copied element.
         *
         * @requires T is Clonable or Copyable.
         */
        Option<T> get_clone(Size idx) const noexcept
            requires Clonable<T> or Copyable<T>;

        /**
         * @brief Move-assigns all elements from another array.
         *
         * Each element is move-assigned individually. Self-assignment is ignored.
         *
         * @param other Source array.
         * @return Reference to this array.
         *
         * @requires T is MoveAssignable.
         */
        Array& operator=(Array&& other) noexcept
            requires MoveAssignable<T>;
    };

}


#include "Array.inl"


#endif // SURE_COMMON_TYPES_ARRAY_HPP