#ifndef SURE_COMMON_UTILS_COLLECTIONS_STATICS_STATIC_VEC_HPP
#define SURE_COMMON_UTILS_COLLECTIONS_STATICS_STATIC_VEC_HPP

#ifndef UTILS_COLLECTIONS_STATICS_STATIC_VEC_HPP
#error "Do not include this file directly"
#endif // UTILS_COLLECTIONS_STATICS_STATIC_VEC_HPP


namespace sure::collections {

    template<typename T, Size CAP>
    Void StaticVec<T, CAP>::_placement_new(const T& val, const Size idx) & noexcept requires (Clonable<T> or Copyable<T>) {
        if constexpr (Clonable<T>) {
            new (this->_slot_ptr(idx)) T(val.clone());
        }
        else {
            new (this->_slot_ptr(idx)) T(val);
        }
        this->_length += 1;
    }


    template<typename T, Size CAP>
    Void StaticVec<T, CAP>::_placement_new(T&& val, Size idx) & noexcept requires MoveConstructible<T> {
        new (this->_slot_ptr(idx)) T(do_move(val));
        this->_length += 1;
    }


    template<typename T, Size CAP>
    template<typename ...Args, Size ...Idx>
    Void StaticVec<T, CAP>::_placement_new(IndexSeq<Idx...>, Args&&... args) & noexcept {
        (new (this->_slot_ptr(Idx)) T(do_forward<Args>(args)), ...);
        this->_length = sizeof...(Args);
    }


    template<typename T, Size CAP>
    StaticVec<T, CAP> StaticVec<T, CAP>::_impl_clone() const& noexcept
        requires (Clonable<T> or Copyable<T>)
    {
        mut new_one = StaticVec();
        if constexpr (Clonable<T>) {
            for (mut i = 0; i < this->_length; i++)
                new_one.push(this->_slot_ptr(i)->clone());
        }
        else {
            for (mut i = 0; i < this->_length; i++)
                new_one.push(*this->_slot_ptr(i));
        }

        return new_one;
    }


    template<typename T, Size CAP>
    template<typename ...Args>
        requires (sizeof...(Args) <= CAP)
    StaticVec<T, CAP>::StaticVec(Args&&... args) noexcept {
        this->_placement_new(MakeIdxSeq<sizeof...(Args)>{}, do_forward<Args>(args)...);
    }


    template<typename T, Size CAP>
    StaticVec<T, CAP>::StaticVec(StaticVec&& other) noexcept
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
                src->~T();
            }
        }
        this->_length = other._length;
        other._length = 0;
    }


    template<typename T, Size CAP>
    StaticVec<T, CAP>::~StaticVec() noexcept {
        for (mut i = 0; i < this->_length; i++) {
            this->_slot_ptr(i)->~T();
        }
        this->_length = 0;
    }


    template<typename T, Size CAP>
    NODISCARD_ StaticVec<T, CAP> StaticVec<T, CAP>::clone() const& noexcept
        requires (Clonable<T> or Copyable<T>)
    {
        return this->_impl_clone();
    };


    template<typename T, Size CAP>
    T& StaticVec<T, CAP>::operator[](Size idx) & noexcept {
        CHECK_ABORT(idx >= this->_length, "StaticVec index out of range.");
        return *this->_slot_ptr(idx);
    }


    template<typename T, Size CAP>
    const T& StaticVec<T, CAP>::operator[](Size idx) const& noexcept {
        CHECK_ABORT(idx >= this->_length, "StaticVec index out of range.");
        return *this->_slot_ptr(idx);
    }


    template<typename T, Size CAP>
    T StaticVec<T, CAP>::at_clone(Size idx) & noexcept
        requires Clonable<T> or Copyable<T>
    {
        CHECK_ABORT(idx >= this->_length, "StaticVec index out of range.");
        if constexpr (Clonable<T>) {
            return this->_slot_ptr(idx)->clone();
        }
        else {
            return *this->_slot_ptr(idx);
        }
    }


    template<typename T, Size CAP>
    OptionRef<T> StaticVec<T, CAP>::get_ref(Size idx) & noexcept {
        return idx < this->_length ? OptionRef<T>::some(*this->_slot_ptr(idx)) : OptionRef<T>::none();
    }


    template<typename T, Size CAP>
    OptionRef<const T> StaticVec<T, CAP>::get_ref(Size idx) const& noexcept {
        return idx < this->_length ? OptionRef<const T>::some(*this->_slot_ptr(idx)) : OptionRef<const T>::none();
    }


    template<typename T, Size CAP>
    Option<T> StaticVec<T, CAP>::get_clone(Size idx) const noexcept
        requires Clonable<T> or Copyable<T>
    {
        if constexpr (Clonable<T>) {
            return idx < this->_length ? Option<T>::some(this->_slot_ptr(idx)->clone()) : Option<T>::none();
        }
        else {
            return idx < this->_length ? Option<T>::some(T(*this->_slot_ptr(idx))) : Option<T>::none();
        }
    }


    template<typename T, Size CAP>
    Void StaticVec<T, CAP>::push(const T& value) & noexcept requires (Clonable<T> or Copyable<T>) {
        CHECK_ABORT(this->_length >= CAP, "Cannot push: StaticVec is at full capacity.");
        this->_placement_new(value, this->_length);
    }


    template<typename T, Size CAP>
    Void StaticVec<T, CAP>::push(T&& value) & noexcept requires MoveConstructible<T> {
        CHECK_ABORT(this->_length >= CAP, "Cannot push: StaticVec is at full capacity.");
        this->_placement_new(do_move(value), this->_length);
    }


    template<typename T, Size CAP>
    NODISCARD_ Bool StaticVec<T, CAP>::try_push(const T& value) & noexcept requires (Clonable<T> or Copyable<T>) { // TODO: Convert return type to `Result`
        if (this->_length >= CAP) return false;
        this->_placement_new(value, this->_length);
        return true;
    }


    template<typename T, Size CAP>
    NODISCARD_ Bool StaticVec<T, CAP>::try_push(T&& value) & noexcept requires MoveConstructible<T> { // TODO: Convert return type to `Result`
        if (this->_length >= CAP) return false;
        this->_placement_new(do_move(value), this->_length);
        return true;
    }


    template<typename T, Size CAP>
    NODISCARD_ T StaticVec<T, CAP>::pop() & noexcept requires MoveConstructible<T> {
        CHECK_ABORT(this->_length == 0, "Cannot pop: StaticVec is empty.");

        this->_length -= 1;
        let slot = this->_slot_ptr(this->_length);
        mut result = do_move(*slot);
        slot->~T();

        return result;
    }


    template<typename T, Size CAP>
    NODISCARD_ Option<T> StaticVec<T, CAP>::try_pop() & noexcept requires MoveConstructible<T> { // TODO: Convert return type to `Result`
        if (this->_length == 0) return Option<T>::none();

        this->_length -= 1;
        let slot = this->_slot_ptr(this->_length);
        mut result = do_move(*slot);
        slot->~T();

        return Option<T>::some(do_move(result));
    }


    template<typename T, Size CAP>
    StaticVec<T, CAP>& StaticVec<T, CAP>::operator=(StaticVec&& other) noexcept
        requires (MoveAssignable<T> or TriviallyMoveAssignable<T>)
    {
        if (this == &other) {
            return *this;
        }

        if (other._length == 0) {
            for (mut i = 0; i < this->_length; i++) {
                this->_slot_ptr(i)->~T();
            }
            this->_length = 0;
            return *this;
        }

        if constexpr (TriviallyMoveConstructible<T>) {
            mem_copy(this->_storage, other._storage, other._length * sizeof(T));
        }
        else {
            let less = this->_length <= other._length ? this->_length : other._length;
            let extra = this->_length <= other._length ? other._length : this->_length;

            for (mut i = 0; i < less; i++) {
                *this->_slot_ptr(i) = do_move(*other._slot_ptr(i));
                other._slot_ptr(i)->~T();
            }

            for (mut i = less; i < extra; ++i) {
                if (this->_length <= other._length) {
                    if constexpr (MoveConstructible<T>) { // TODO: Separate this inline-check to different overload method
                        this->push(do_move(*other._slot_ptr(i)));
                        other._slot_ptr(i)->~T();
                    }
                    else {
                        ABORT("StaticVec move-assignment cannot grow: T is move-assignable but not move-constructible, so no new slots can be constructed.");
                    }
                }
                else {
                    this->_slot_ptr(i)->~T();
                }
            }
        }
        this->_length = other._length;
        other._length = 0;

        return *this;
    }

}


#endif // SURE_COMMON_UTILS_COLLECTIONS_STATICS_STATIC_VEC_HPP