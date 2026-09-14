#ifndef SURE_COMMON_TYPES_ARRAY_INL
#define SURE_COMMON_TYPES_ARRAY_INL

#ifndef SURE_COMMON_TYPES_ARRAY_HPP
#error "Do not include this file directly"
#endif // SURE_COMMON_TYPES_ARRAY_HPP


namespace sure {

    template<typename T, Size N>
    template<Size... Idx>
        requires Clonable<T> or CopyConstructible<T>
    Array<T, N> Array<T, N>::_clone_impl(IndexSeq<Idx...>) const& noexcept {
        if constexpr (Clonable<T>) {
            return Array { this->_data[Idx].clone() ... };
        }
        else {
            return Array { this->_data[Idx] ... };
        }
    }


    template<typename T, Size N>
    NODISCARD_ Array<T, N> Array<T, N>::clone() const& noexcept
        requires (Clonable<T> or CopyConstructible<T>)
    {
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


    template<typename T, Size N>
    T& Array<T, N>::operator[](Size idx) & noexcept {
        CHECK_ABORT(idx >= N, "Array length exceeded.");
        return this->_data[idx];
    }


    template<typename T, Size N>
    const T& Array<T, N>::operator[](Size idx) const& noexcept {
        CHECK_ABORT(idx >= N, "Array length exceeded.");
        return this->_data[idx];
    }


    template<typename T, Size N>
    T Array<T, N>::at_clone(Size idx) const noexcept
        requires Clonable<T> or Copyable<T>
    {
        CHECK_ABORT(idx >= N, "Array length exceeded.");
        if constexpr (Clonable<T>) {
            return this->_data[idx].clone();
        }
        else {
            return this->_data[idx];
        }
    }


    template<typename T, Size N>
    OptionRef<T> Array<T, N>::get_ref(Size idx) & noexcept {
        return idx < N ? OptionRef<T>::some(this->_data[idx]) : OptionRef<T>::none();
    }


    template<typename T, Size N>
    OptionRef<const T> Array<T, N>::get_ref(Size idx) const& noexcept {
        return idx < N ? OptionRef<const T>::some(this->_data[idx]) : OptionRef<const T>::none();
    }


    template<typename T, Size N>
    Option<T> Array<T, N>::get_clone(Size idx) const noexcept
        requires Clonable<T> or Copyable<T>
    {
        if constexpr (Clonable<T>) {
            return idx < N ? Option<T>::some(this->_data[idx].clone()) : Option<T>::none();
        }
        else {
            return idx < N ? Option<T>::some(T(this->_data[idx])) : Option<T>::none();
        }
    }


    template<typename T, Size N>
    Array<T, N>& Array<T, N>::operator=(Array&& other) noexcept
        requires MoveAssignable<T>
    {
        if (this != &other) {
            this->_move_assign_impl(do_forward<Array&&>(other), MakeIdxSeq<N>());
        }
        return *this;
    }
}


#endif // SURE_COMMON_TYPES_ARRAY_INL