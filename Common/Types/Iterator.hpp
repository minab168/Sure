#ifndef SURE_COMMON_TYPES_ITERATOR_HPP
#define SURE_COMMON_TYPES_ITERATOR_HPP

#include "../Base/Interface/Lang.hpp"
#include "../Utils/Check.hpp"
#include "Primitives/Inc.hpp"

namespace sure {

    template<typename T>
    class Iterator {
        T* _ptr = NULL_PTR;

      public:
        constexpr Iterator() noexcept = default;

        explicit constexpr Iterator(T* ptr) noexcept : _ptr(ptr) {}


        Bool operator!=(const Iterator& it) const noexcept {
            return this->_ptr != it._ptr;
        }


        Bool operator==(const Iterator& it) const noexcept {
            return this->_ptr == it._ptr;
        }


        T& operator*() noexcept {
            CHECK_ABORT(this->_ptr == NULL_PTR, "Null pointer!");
            return *this->_ptr;
        }


        const T& operator*() const noexcept {
            CHECK_ABORT(this->_ptr == NULL_PTR, "Null pointer!");
            return *this->_ptr;
        }


        Iterator& operator++() noexcept {
            ++this->_ptr;
            return *this;
        }


        Iterator& operator--() noexcept {
            --this->_ptr;
            return *this;
        }


        Iterator operator+(const I32 n) const noexcept {
            return Iterator(this->_ptr + n);
        }
    };
}

#endif // SURE_COMMON_TYPES_ITERATOR_HPP