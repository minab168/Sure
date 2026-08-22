#pragma once




#if defined(__CHAR_BIT__)
    #define CHAR_SIZE_ __CHAR_BIT__
#elif defined(SURE__COMPILER_GCC_) || defined(SURE__COMPILER_CLANG_) || defined(SURE__COMPILER_MSVC_) || defined(__AVR__)
    #define CHAR_SIZE_ 8
#else
    #define CHAR_SIZE_ 16
#endif


#if defined(__SIZEOF_SHORT__)
    #define SHORT_SIZE_ (__SIZEOF_SHORT__ * CHAR_SIZE_)
#else
    #define SHORT_SIZE_ 16
#endif


#if defined(__SIZEOF_INT__)
    #define INT_SIZE_ (__SIZEOF_INT__ * CHAR_SIZE_)
#else
    #define INT_SIZE_ 32
#endif


#if defined(__SIZEOF_LONG__)
    #define LONG_SIZE_ (__SIZEOF_LONG__ * CHAR_SIZE_)
#else
    #define LONG_SIZE_ 32
#endif


#if defined(__SIZEOF_LONG_LONG__)
    #define LLONG_SIZE_ (__SIZEOF_LONG_LONG__ * CHAR_SIZE_)
#else
    #define LLONG_SIZE_ 64
#endif


// TODO: Complete this