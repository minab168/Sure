#ifndef SURE_COMMON_BASE_MEMORY_HPP_
#define SURE_COMMON_BASE_MEMORY_HPP_


#include "Compiler.hpp"
#include "../../Types/Primitives/Inc.hpp"


namespace sure::base {
    #if CPP_ALIGNED_NEW_
        enum class AlignVal: Size {};
    #endif

    struct NoThrow {
        #if SURE__CPP11_OR_HIGHER_
            explicit NoThrow() = default;
        #endif
    };

    extern const NoThrow nothrow;
}


#ifdef SURE__NO_HEAP_
    #define HEAP_ALLOC_FORBIDDEN_MESSAGE  "Heap allocation is forbidden!"
    #define HEAP_DELETE_FORBIDDEN_MESSAGE "Heap deletion is forbidden!"


    NODISCARD_ Void* operator new(Size);

    NODISCARD_ Void* operator new[](Size);

    NODISCARD_ Void* operator new(Size, const sure::base::NoThrow&) noexcept;

    NODISCARD_ Void* operator new[](Size, const sure::base::NoThrow&) noexcept;


    Void operator delete(Void*) noexcept;

    Void operator delete[](Void*) noexcept;


    Void operator delete(Void*, const sure::base::NoThrow&) noexcept;

    Void operator delete[](Void*, const sure::base::NoThrow&) noexcept;


    #if CPP_SIZED_DEALLOCATION_
        Void operator delete(Void*, Size) noexcept;

        Void operator delete[](Void*, Size) noexcept;
    #endif

    #if CPP_ALIGNED_NEW_
        NODISCARD_ Void* operator new(Size, sure::base::AlignVal);

        NODISCARD_ Void* operator new(Size, sure::base::AlignVal, const sure::base::NoThrow&);

        NODISCARD_ Void* operator new[](Size, sure::base::AlignVal);

        NODISCARD_ Void* operator new[](Size, sure::base::AlignVal, const sure::base::NoThrow&);


        Void operator delete(Void*, sure::base::AlignVal) noexcept;

        Void operator delete(Void*, sure::base::AlignVal, const sure::base::NoThrow&) noexcept;

        Void operator delete[](Void*, sure::base::AlignVal) noexcept;

        Void operator delete[](Void*, sure::base::AlignVal, const sure::base::NoThrow&) noexcept;


        #if CPP_SIZED_DEALLOCATION_
            Void operator delete(Void*, Size, sure::base::AlignVal) noexcept;

            Void operator delete[](Void*, Size, sure::base::AlignVal) noexcept;
        #endif
    #endif
#endif


#endif // SURE_COMMON_BASE_MEMORY_HPP_
