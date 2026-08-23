#ifndef SURE_COMMON_UTILS_LOCATION_INC_HPP_
#define SURE_COMMON_UTILS_LOCATION_INC_HPP_


#include "../../Base/Compiler.hpp"


#if SURE__HAS_BUILTIN_(__builtin_FILE)
    #define FILE_ __builtin_FILE()
#else
    #define FILE_ __FILE__
#endif


#if SURE__HAS_BUILTIN_(__builtin_LINE)
    #define LINE_ __builtin_LINE()
#else
    #define LINE_ __LINE__
#endif


#if SURE__HAS_BUILTIN_(__builtin_COLUMN)
    #define COLUMN_ __builtin_COLUMN()
#else
    #define COLUMN_ 0
#endif


#if SURE__HAS_BUILTIN_(__builtin_FUNCTION)
    #define FUNCTION_ __builtin_FUNCTION()
#elif defined(__func__)
    #define FUNCTION_ __func__
#elif defined(__FUNCTION__)
    #define FUNCTION_ __FUNCTION__
#else
    #define FUNCTION_ "<unknown>"
#endif


#if SURE__COMPILER_CLANG_ || SURE__COMPILER_GCC_
    #define PRETTY_FUNCTION_ __PRETTY_FUNCTION__
#elif SURE__COMPILER_MSVC_
    #define PRETTY_FUNCTION_ __FUNCSIG__
#elif defined(__FUNCTION__)
    #define PRETTY_FUNCTION_ __FUNCTION__
#else
    #define PRETTY_FUNCTION_ "<unknown>"
#endif


#endif // SURE_COMMON_UTILS_LOCATION_INC_HPP_