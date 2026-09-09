#ifndef SURE_COMMON_BASE_COMPILER_HPP_
#define SURE_COMMON_BASE_COMPILER_HPP_


#include "./Platform.hpp"


// -------------------------------- Compiler version --------------------------------


#if SURE__COMPILER_GCC_
    #define SURE__COMPILER_VERSION_ (__GNU__ * 100 + __GNUC_MINOR__)
#elif SURE__COMPILER_CLANG_
    #define SURE__COMPILER_VERSION_ (__clang_major__ * 100 + __clang_minor__)
#elif SURE__COMPILER_MSVC_
    #define SURE__COMPILER_VERSION_ _MSC_VER
#endif


// ---------------------------------- CPP version -----------------------------------


#ifdef __cplusplus
    #define SURE__IS_CPP_
#endif


#ifdef SURE__IS_CPP_
    #define SURE__CPP_VERSION_     __cplusplus
    #define SURE__CPP98_OR_HIGHER_ (CPP_VERSION_ >= 199711L)
    #define SURE__CPP11_OR_HIGHER_ (CPP_VERSION_ >= 201103L)
    #define SURE__CPP14_OR_HIGHER_ (CPP_VERSION_ >= 201402L)
    #define SURE__CPP17_OR_HIGHER_ (CPP_VERSION_ >= 201703L)
    #define SURE__CPP20_OR_HIGHER_ (CPP_VERSION_ >= 202002L)
    #define SURE__CPP23_OR_HIGHER_ (CPP_VERSION_ >= 202302L)
#else
    #define SURE__CPP_VERSION_     0
    #define SURE__CPP98_OR_HIGHER_ 0
    #define SURE__CPP11_OR_HIGHER_ 0
    #define SURE__CPP14_OR_HIGHER_ 0
    #define SURE__CPP17_OR_HIGHER_ 0
    #define SURE__CPP20_OR_HIGHER_ 0
    #define SURE__CPP23_OR_HIGHER_ 0
#endif



// ------------------------------- Calling convention -------------------------------


#if SURE__COMPILER_MSVC_
    #define CDECL_    __cdecl
    #define STDCALL_  __stdcall
    #define FASTCALL_ __fastcall
#else
    #define CDECL_
    #define STDCALL_
    #define FASTCALL_
#endif


// ------------------------------- Alignment / Packed -------------------------------


#if SURE__COMPILER_MSVC_
    #define ALIGN_(n_)              __declspec(align(n_))
    #define PACKED_(decl_)          _pragma(pack(push,1)) decl_ __pragma(pack(pop))
    #define CPP_ALIGNED_NEW
    #define CPP_SIZED_DEALLOCATION_
#elif SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_ || SURE__COMPILER_INTEL_
    #define ALIGN_(n_)              __attribute__((aligned(n_)))
    #define PACKED_(decl_) decl_    __attribute__((packed))
    #define CPP_ALIGNED_NEW_        __cpp_aligned_new
    #define CPP_SIZED_DEALLOCATION_ __cpp_sized_deallocation
#else
    #define ALIGN_(n_)
    #define PACKED_(decl_) decl_
    #define CPP_ALIGNED_NEW
    #define CPP_SIZED_DEALLOCATION_
#endif


// ----------------------------- Optimization / Helpers -----------------------------


#if SURE__COMPILER_MSVC_
    #if (SURE__COMPILER_VERSION_ >= 1200)
        #define FORCE_INLINE_ __forceinline
    #else
        #define FORCE_INLINE_ __inline
    #endif
#elif SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_ || SURE__COMPILER_INTEL_
    #define FORCE_INLINE_ inline __attribute__((always_inline))
#elif defined(SURE__COMPILER_INTEL_)
    #define FORCE_INLINE_ __forceinline
#else
    #define FORCE_INLINE_ inline
#endif


#if SURE__COMPILER_MSVC_
    #define NO_INLINE_ __declspec(noinline)
#elif SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_ || SURE__COMPILER_INTEL_
    #define NO_INLINE_ inline __attribute__((noinline))
#else
    #define NO_INLINE_
#endif


#if SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_
    #define LIKELY_(x_)   __builtin_expect(!!(x_), 1)
    #define UNLIKELY_(x_) __builtin_expect(!!(x_), 0)
#else
    #define LIKELY_(x_)   (x_)
    #define UNLIKELY_(x_) (x_)
#endif


#if SURE__COMPILER_MSVC_
    #define DEPRECATED(msg_) __declspec(deprecated(msg_))
#elif SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_ || SURE__COMPILER_INTEL_
    #define DEPRECATED_(msg_) __attribute__((deprecated(msg_)))
#else
    #define DEPRECATED_(msg_)
#endif


#if SURE__COMPILER_MSVC_
    #pragma warning(error: 4996)
    #define UNAVAILABLE(msg_) __declspec(unavaliable(msg_))
#elif SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_ || SURE__COMPILER_INTEL_
    #define UNAVAILABLE_(msg_) __attribute__((unavailable(msg_)))
#else
    #define UNAVAILABLE_(msg_)
#endif


#if SURE__COMPILER_MSVC_
    #define NODISCARD_ _Check_return
#elif SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_
    #define NODISCARD_ __attribute__((warn_unused_result))
#else
    #define NODISCARD_
#endif


#if SURE__COMPILER_MSVC_
    #define MAYBE_UNUSED
#elif SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_
    #define MAYBE_UNUSED __attribute__((unused))
#else
    #define MAYBE_UNUSED
#endif


#if SURE__COMPILER_MSVC_
    #define THREAD_LOCAL_ __declspec(thread)
#elif SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_ || SURE__COMPILER_INTEL_
    #define THREAD_LOCAL_ __thread
#else
    #define THREAD_LOCAL_
#endif


#if SURE__COMPILER_MSVC_
    #define do_pragma_(x_) __pragma(#x_)
#elif SURE__COMPILER_GCC_ || SURE__COMPILER_CLANG_ || SURE__COMPILER_INTEL_
    #define do_pragma_(x_) _Pragma (#x_)
#else
    #define do_pragma_(x_)
#endif


#ifndef SURE__HAS_BUILTIN_
    #ifdef __has_builtin
        #define SURE__HAS_BUILTIN_(x) __has_builtin(x)
    #else
        #define SURE__HAS_BUILTIN_(x) 0
    #endif
#endif


#endif // SURE_COMMON_BASE_COMPILER_HPP_