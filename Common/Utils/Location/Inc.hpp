#pragma once

#include <Sure/Common/Base/Compiler.hpp>


#if HAS_BUILTIN_(__builtin_FILE)
    #define FILE_() __builtin_FILE()
#else
    #define FILE_() __FILE__
#endif


#if HAS_BUILTIN_(__builtin_LINE)
    #define LINE_() __builtin_LINE()
#else
    #define LINE_() __LINE__
#endif


#if HAS_BUILTIN_(__builtin_COLUMN)
    #define COLUMN_() __builtin_COLUMN()
#else
    #define COLUMN_() 0
#endif


#if HAS_BUILTIN_(__builtin_FUNCTION)
    #define FUNCTION_() __builtin_FUNCTION()
#elif defined(__func__)
    #define FUNCTION_() __func__
#elif defined(__FUNCTION__)
    #define FUNCTION_() __FUNCTION__
#else
    #define FUNCTION_() "<unknown>"
#endif


#if COMPILER_CLANG_ || COMPILER_GCC_
    #define PRETTY_FUNCTION_() __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define PRETTY_FUNCTION_() __FUNCSIG__
#elif defined(__FUNCTION__)
    #define PRETTY_FUNCTION_() __FUNCTION__
#else
    #define PRETTY_FUNCTION_() "<unknown>"
#endif