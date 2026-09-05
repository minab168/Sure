#ifndef SURE_COMMON_TYPES_INDEX_SEQ_H
#define SURE_COMMON_TYPES_INDEX_SEQ_H

#include "Primitives/Inc.hpp"

namespace sure {

    // TODO: convert to log(n) algorithm

    template<typename T, T... Idx>
    struct IntSeq {
        typedef T ValueType;
        static constexpr Size size() noexcept { return sizeof...(Idx); }
    };

    template<size_t... Idx>
    using IndexSeq = IntSeq<Size, Idx...>;

    template<typename T, T Num>
    using MakeIntSeq =
    #if SURE__HAS_BUILTIN_(__make_integer_seq)
        __make_integer_seq<IntSeq, T, Num>;
    #else
        IntSeq<T, __integer_pack(Num)...>;
    #endif

    template<size_t Num>
    using MakeIdxSeq = MakeIntSeq<Size, Num>;
}

#endif // SURE_COMMON_TYPES_INDEX_SEQ_H