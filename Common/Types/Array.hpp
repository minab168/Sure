//
// Created by user on 12/09/2026.
//

#ifndef SURE_COMMON_TYPES_ARRAY_HPP
#define SURE_COMMON_TYPES_ARRAY_HPP

#include <utility>

#include "IndexSeq.hpp"
#include "Option.hpp"
#include "../Ownership/Inc.hpp"
#include "../Ownership/Move.hpp"
#include "../Traits/Common.hpp"
#include "../Traits/Creation.hpp"
#include "../Utils/Check.hpp"
#include "Primitives/Inc.hpp"


namespace sure {

    template<typename T, Size N>
    class Array: NoDefaultCopy { // Satisfied Clonable
        T _data[N];

        template<Size... Idx>
        requires Clonable<T>
        Array _clone_impl(IndexSeq<Idx...>) const& noexcept {
            return Array { this->_data[Idx].clone() ... };
        }

        template<Size... Idx>
        requires CopyConstructible<T>
        Array _clone_impl(IndexSeq<Idx...>) const& noexcept {
            return Array { this->_data[Idx] ... };
        }

    public:
        constexpr Array() noexcept
            requires DefaultConstructible<T> = default;

        template<typename ...Args>
            requires (sizeof...(Args) == N)
        constexpr explicit Array(Args&&...args) noexcept:
            _data{static_cast<T>(do_forward<Args>(args))...}
        { }


        Array clone() const& noexcept requires (Clonable<T> or CopyConstructible<T>) {
            #if !SURE__HAS_BUILTIN_(__make_integer_seq)
                static_assert(
                    N <= 512,
                    "Array<T,N> with N . 512 needs compiler builtin support (__make_integer_seq)"
                    "for index-sequence generation, or a higher -ftemplate-depth if your compiler"
                    "supports the recursive fallback."
                );
            #endif
            return this->_clone_impl(MakeIdxSeq<N>{});
        }


        T operator [](Size idx) const& noexcept {
            CHECK_ABORT(idx > N, "Array length exceeded.");
            return this->_data[idx];
        }


        Option<T> get(Size idx) const& noexcept {
            return idx <= N ? some_ty_(this->_data[idx], T) : none_(T);
        }
    };

}


#endif // SURE_COMMON_TYPES_ARRAY_HPP