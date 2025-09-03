#pragma once

#include "./Platform.hpp"


// -------------------------------- Compiler version --------------------------------


#if COMPILER_GCC_
    #define COMPILER_VERSION_ (__GNU__ * 100 + __GNUC_MINOR__)
#elif COMPILER_CLANG_
    #define COMPILER_VERSION_ (__clang_major__ * 100 + __clang_minor__)
#elif COMPILER_MSVC_
    #define COMPILER_VERSION_ _MSC_VER
#endif


// ---------------------------------- CPP version -----------------------------------


#if defined(__cplusplus)
    #define IS_CPP_ 1
#else 
    #define IS_CPP_ 0
#endif


#if IS_CPP_
    #define CPP_VERSION_     __cplusplus
    #define CPP98_OR_HIGHER_ (CPP_VERSION_ >= 199711L)
    #define CPP11_OR_HIGHER_ (CPP_VERSION_ >= 201103L)
    #define CPP14_OR_HIGHER_ (CPP_VERSION_ >= 201402L)
    #define CPP17_OR_HIGHER_ (CPP_VERSION_ >= 201703L)
    #define CPP20_OR_HIGHER_ (CPP_VERSION_ >= 202002L)
    #define CPP23_OR_HIGHER_ (CPP_VERSION_ >= 202302L)
#else
    #define CPP_VERSION_     0
    #define CPP98_OR_HIGHER_ 0
    #define CPP11_OR_HIGHER_ 0
    #define CPP14_OR_HIGHER_ 0
    #define CPP17_OR_HIGHER_ 0
    #define CPP20_OR_HIGHER_ 0
    #define CPP23_OR_HIGHER_ 0
#endif



// ------------------------------- Calling convention -------------------------------


#if COMPILER_MSVC_
    #define CDECL_    __cdecl
    #define STDCALL_  __stdcall
    #define FASTCALL_ __fastcall
#else
    #define CDECL_
    #define STDCALL_
    #define FASTCALL_
#endif


// ------------------------------- Alignment / Packed -------------------------------


#if COMPILER_MSVC_
    #define ALIGN_(n_)     __declspec(align(n_))
    #define PACKED_(decl_) _pragma(pack(push,1)) decl_ __pragma(pack(pop))
#elif COMPILER_GCC_ || COMPILER_CLANG_ || COMPILER_INTEL_
    #define ALIGN_(n_)     __attribute__((aligned(n_)))
    #define PACKED_(decl_) decl_ __attribute__((packed))
#else
    #define ALIGN_(n_)     
    #define PACKED_(decl_) decl_
#endif


// ----------------------------- Optimization / Helpers -----------------------------


#if COMPILER_MSVC_
    #if (COMPILER_VERSION_ >= 1200)
        #define FORCE_INLINE_ __forceinline
    #else
        #define FORCE_INLINE_ __inline
    #endif
#elif COMPILER_GCC_ || COMPILER_CLANG_ || COMPILER_INTEL_
    #define FORCE_INLINE_ inline __attribute__((always_inline))
#elif defined(COMPILER_INTEL_)
    #define _FORCE_INLINE __forceinline
#else
    #define FORCE_INLINE_ inline
#endif


#if COMPILER_MSVC_
    #define NO_INLINE_ __declspec(noinline)
#elif COMPILER_GCC_ || COMPILER_CLANG_ || COMPILER_INTEL_
    #define NO_INLINE_ inline __attribute__((noinline))
#else
    #define NO_INLINE_
#endif


#if COMPILER_GCC_ || COMPILER_CLANG_
    #define LIKELY_(x_)   __builtin_expect(!!(x_), 1)
    #define UNLIKELY_(x_) __builtin_expect(!!(x_), 0)
#else
    #define LIKELY_(x_)   (x_)
    #define UNLIKELY_(x_) (x_)
#endif


#if COMPILER_MSVC_
    #define DEPRECATED(msg_) __declspec(deprecated(msg_))
#elif COMPILER_GCC_ || COMPILER_CLANG_ || COMPILER_INTEL_
    #define DEPRECATED(msg_) __attribute__((deprecated(msg_)))
#else
    #define DEPRECATED(msg_)
#endif


#if COMPILER_MSVC_
    #define NODISCARD_ _Check_return
#elif COMPILER_GCC_ || COMPILER_CLANG_ 
    #define NODISCARD_ __attribute__((warn_unused_result))
#else
    #define NODISCARD_
#endif


#if COMPILER_MSVC_
    #define MAYBE_UNUSED
#elif COMPILER_GCC_ || COMPILER_CLANG_ 
    #define MAYBE_UNUSED __attribute__((unused))
#else
    #define MAYBE_UNUSED
#endif


#if COMPILER_MSVC_
    #define THREAD_LOCAL_ __declspec(thread)
#elif COMPILER_GCC_ || COMPILER_CLANG_ || COMPILER_INTEL_
    #define THREAD_LOCAL_ __thread
#else
    #define THREAD_LOCAL_
#endif
