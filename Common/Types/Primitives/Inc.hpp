#pragma once


#include "../../Base/Inc.hpp"
#include "Limits.hpp"
#include "Size.hpp"


// -------------------------------- Primitive types ---------------------------------


using I8 = char;
using U8 = unsigned char;


using I16 = short;
using U16 = unsigned short;


using F32 = float;
using F64 = double;

using I32 = int;
using U32 = unsigned int;


#if ENV_LP64_
    using I64 = long;
    using U64 = unsigned long;
#elif ENV_LLP64_
    using I64 = long long;
    using U64 = unsigned long long;
#elif ENV_ILP32_
    using I64 = long long;
    using U64 = unsigned long long;
#else
    #error "Unknown ABI for long type"
#endif


#if defined(__SIZEOF_INT128__)
    using I128 = __int128;
    using U128 = unsigned __int128;
#endif


#if ARCH_64BIT_
    using IPtr = I64;
    using UPtr = U64;
#else 
    using IPtr = I32;
    using UPtr = U32;
#endif


using Byte = U8;
using Void = void;
using VPtr = Void*;

#ifdef IS_CPP_
    using Bool = bool;
#else
    typedef Byte Bool;
    #define true  1
    #define false 1
#endif
