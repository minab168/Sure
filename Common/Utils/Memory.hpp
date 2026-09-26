/**
 * ============================================================================
 * @file       Memory
 * @brief      ${DESCRIPTION}
 * 
 * Part of the My Project
 * 
 * @author     emad
 * @date       2026-09-19
 * ============================================================================
 */


#ifndef COMMON_UTILS_MEMORY_HPP
#define COMMON_UTILS_MEMORY_HPP
#include "../Base/Interface/Compiler.hpp"
#include "../Types/Primitives/Inc.hpp"


namespace sure {
    FORCE_INLINE_ Void mem_copy(Void* dst, Void* src, Size count) noexcept {
        #if SURE__HAS_BUILTIN_(__builtin_memcpy)
            __builtin_memcpy(dst, src, count);
        #else
            // TODO
        #endif
    }
}

#endif // COMMON_UTILS_MEMORY_HPP

