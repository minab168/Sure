#pragma once


#include "../../Base/Interface/Compiler.hpp"


// -------------------------------- Primitive types ---------------------------------


using I8   = char;
using U8   = unsigned char;
using V_I8 = volatile char;
using V_U8 = volatile unsigned char;


using I16   = short;
using U16   = unsigned short;
using V_I16 = volatile short;
using V_U16 = volatile unsigned short;


using F32   = float;
using F64   = double;
using V_F32 = volatile float;
using V_F64 = volatile double;

using I32   = int;
using U32   = unsigned int;
using V_I32 = volatile int;
using V_U32 = volatile unsigned int;


#if SURE__ENV_LP64_
    using I64   = long;
    using U64   = unsigned long;
    using V_I64 = volatile long;
    using V_U64 = volatile unsigned long;
#elif SURE__ENV_LLP64_
    using I64   = long long;
    using U64   = unsigned long long;
    using V_I64 = volatile long long;
    using V_U64 = volatile unsigned long long;
#elif SURE__ENV_ILP32_
    using I64   = long long;
    using U64   = unsigned long long;
    using V_I64 = volatile long long;
    using V_U64 = volatile unsigned long long;
#else
    #error "Unknown ABI for long type"
#endif


#if defined(__SIZEOF_INT128__)
    using I128   = __int128;
    using U128   = unsigned __int128;
    using V_I128 = volatile __int128;
    using V_U128 = volatile unsigned __int128;
#endif


#if SURE__ARCH_64BIT_
    using IPtr   = I64;
    using UPtr   = U64;
    using V_IPtr = volatile I64;
    using V_UPtr = volatile U64;
#else 
    using IPtr   = I32;
    using UPtr   = U32;
    using V_IPtr = volatile I32;
    using V_UPtr = volatile U32;
#endif


using Byte   = U8;
using Char   = char;
using Void   = void;
using VPtr   = Void*;
using V_Byte = volatile U8;
using V_Char = volatile char;
using V_Void = volatile void;
using V_VPtr = volatile Void*;

using Size    = U64;
using SSize   = I64;
using V_Size  = volatile U64;
using V_SSize = volatile I64;

#ifdef SURE__IS_CPP_
    using Bool   = bool;
    using V_Bool = volatile bool;
#else
    typedef Byte          Bool;
    typedef volatile Byte V_Bool;
    #define true          1
    #define false         1
#endif
