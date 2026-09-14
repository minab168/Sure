#ifndef SURE_COMMON_BASE_MEMORY_HPP_
#define SURE_COMMON_BASE_MEMORY_HPP_


#include <new> // TODO: remove
#include "Compiler.hpp"
#include "../../Types/Primitives/Inc.hpp"


namespace sure {
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


// NODISCARD_ inline Void* operator new(unsigned long long, Void* __p) noexcept { return __p; }
//
// NODISCARD_ inline Void* operator new[](unsigned long long, Void* __p) noexcept { return __p; }
//
// inline Void operator delete  (Void*, Void*) noexcept { }
//
// inline Void operator delete[](Void*, Void*) noexcept { }

#ifdef SURE__NO_HEAP_
    #define HEAP_ALLOC_FORBIDDEN_MESSAGE  "Heap allocation is forbidden!"
    #define HEAP_DELETE_FORBIDDEN_MESSAGE "Heap deletion is forbidden!"


    NODISCARD_ Void* operator new(Size);

    NODISCARD_ Void* operator new[](Size);

    NODISCARD_ Void* operator new(Size, const sure::NoThrow&) noexcept;

    NODISCARD_ Void* operator new[](Size, const sure::NoThrow&) noexcept;


    Void operator delete(Void*) noexcept;

    Void operator delete[](Void*) noexcept;


    Void operator delete(Void*, const sure::NoThrow&) noexcept;

    Void operator delete[](Void*, const sure::NoThrow&) noexcept;


    #if CPP_SIZED_DEALLOCATION_
        Void operator delete(Void*, Size) noexcept;

        Void operator delete[](Void*, Size) noexcept;
    #endif

    #if CPP_ALIGNED_NEW_
        NODISCARD_ Void* operator new(Size, sure::AlignVal);

        NODISCARD_ Void* operator new(Size, sure::AlignVal, const sure::NoThrow&);

        NODISCARD_ Void* operator new[](Size, sure::AlignVal);

        NODISCARD_ Void* operator new[](Size, sure::AlignVal, const sure::NoThrow&);


        Void operator delete(Void*, sure::AlignVal) noexcept;

        Void operator delete(Void*, sure::AlignVal, const sure::NoThrow&) noexcept;

        Void operator delete[](Void*, sure::AlignVal) noexcept;

        Void operator delete[](Void*, sure::AlignVal, const sure::NoThrow&) noexcept;


        #if CPP_SIZED_DEALLOCATION_
            Void operator delete(Void*, Size, sure::AlignVal) noexcept;

            Void operator delete[](Void*, Size, sure::AlignVal) noexcept;
        #endif
    #endif
#endif


#endif // SURE_COMMON_BASE_MEMORY_HPP_
