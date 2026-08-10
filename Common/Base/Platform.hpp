#pragma once



// ------------------------------- Compiler detection -------------------------------

#if defined(_MSC_VER)
    #define COMPILER_MSVC_ 1
#else
    #define COMPILER_MSVC_ 0
#endif


#if defined(__GNUC__) && !defined(__clang__)
    #define COMPILER_GCC_ 1
#else
    #define COMPILER_GCC_ 0
#endif


#if defined(__clang__)
    #define COMPILER_CLANG_ 1
#else
    #define COMPILER_CLANG_ 0
#endif


#if defined(__INTEL_COMPILER)
    #define COMPILER_INTEL_ 1
#else
    #define COMPILER_INTEL_ 0
#endif


#if !COMPILER_MSVC_ && !COMPILER_GCC_ && !COMPILER_CLANG_ && !COMPILER_INTEL_
    #error "Unsupported compiler"
#endif


// ---------------------------------- OS detection ----------------------------------


#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS_ 1
#else
    #define OS_WINDOWS_ 0
#endif


#if defined(__linux__)
    #define OS_LINUX_ 1
#else
    #define OS_LINUX_ 0
#endif


#if defined(__APPLE__) && defined(__MACH__)
    #define OS_MAC_ 1
#else
    #define OS_MAC_ 0
#endif


#if defined(__FreeBSD__)
    #define OS_FREEBSD_ 1
#else
    #define OS_FREEBSD_ 0
#endif


#if defined(CONFIG_IDF_TARGET_ESP32) || defined(ESP_PLATFORM)
    #define OS_ESP_IDF 1
#else
    #define OS_ESP_IDF 0
#endif


#if !OS_WINDOWS_ && !OS_LINUX_ && !OS_MAC_ && !OS_FREEBSD_ && !OS_ESP_IDF
    #error "Unsupported OS platform"
#endif


// ----------------------------- Architecture detection -----------------------------


#if defined(__x86_64__) || defined(_M_X64)
    #define ARCH_X64_ 1
#else
    #define ARCH_X64_ 0
#endif


#if defined(__i386__) || defined(_M_IX86)
    #define ARCH_X86_ 1
#else
    #define ARCH_X86_ 0
#endif


#if defined(__aarch64__) || defined(_M_ARM64)
    #define ARCH_ARM64_ 1
#else
    #define ARCH_ARM64_ 0
#endif


#if defined(__arm__) || defined(_M_ARM)
    #define ARCH_ARM32_ 1
#else
    #define ARCH_ARM32_ 0
#endif


#if defined(__powerpc64__) || defined(__ppc64__)
    #define ARCH_PPC64_ 1
#else
    #define ARCH_PPC64_ 0
#endif


#if defined(__powerpc__) || defined(__ppc__)
    #define ARCH_PPC32_ 1
#else
    #define ARCH_PPC32_ 0
#endif


#if ARCH_X64_ || ARCH_ARM64_ || ARCH_PPC64_
    #define ARCH_64BIT_ 1
#else
    #define ARCH_64BIT_ 0
#endif


#if !ARCH_64BIT_
    #define ARCH_32BIT_ 1
#else
    #define ARCH_32BIT_ 0
#endif


#if defined(__xtensa__)
    #define ARCH_XTENSA_ 1
#else
    #define ARCH_XTENSA_ 0
#endif


#if defined(__riscv) || defined(__riscv__)
    #define ARCH_RISCV_ 1
#else
    #define ARCH_RISCV_ 0
#endif


#if !ARCH_X86_ \
    && !ARCH_X64_ \
    && !ARCH_ARM64_ \
    && !ARCH_ARM32_ \
    && !ARCH_PPC64_ \
    && !ARCH_PPC32_ \
    && !ARCH_XTENSA_ \
    && !ARCH_RISCV_
    #error "Unsupported architecture"
#endif


// ------------------------------ Endianness detection ------------------------------


#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
    #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define ENDIAN_BIG_   1
        #define ENDIAN_LITTLE 0
    #else
        #define ENDIAN_BIG_   0
        #define ENDIAN_LITTLE 1
    #endif
#else
    #define ENDIAN_BIG_   0
    #define ENDIAN_LITTLE 1
#endif


// --------------------------------- ABI detection ----------------------------------


#if defined(_WIN32) || defined(_WIN64)
    #define ENV_LLP64_ 1
#else
    #define ENV_LLP64_ 0
#endif


#if defined(__LP64__) || defined(__x86_64__) || defined(__aarch64__)
    #define ENV_LP64_ 1
#else
    #define ENV_LP64_ 0
#endif


#if !ENV_LP64 && !ENV_LLP64
    #define ENV_ILP32_ 1
#else
    #define ENV_ILP32_ 0
#endif