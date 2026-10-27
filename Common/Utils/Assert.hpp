#ifndef SURE_COMMON_BASE_ASSERT_HPP_
#define SURE_COMMON_BASE_ASSERT_HPP_


#include "../Base/Interface/Lang.hpp"
#include "./Logging/Inc.hpp"
#include "./Abort.hpp"

#define SURE_STATIC_ASSERT(cond, msg) static_assert(cond, msg)

#define SURE_IS_TRUE(cond)  (!!cond)
#define SURE_IS_FALSE(cond) (!cond)

#define SURE_IS_NULL(cond)  (cond == NULL_PTR)

#define SURE_IS_VALID_PTR(cond) !SURE_IS_NULL(cond)
#define SURE_IS_INV_PTR(cond)   SURE_IS_NULL(cond)


#define SURE_ABORT_ON_TRUE(cond, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_TRUE(cond))) { \
            SURE_ABORT(msg, ##__VA_ARGS__); \
        } \
    } while (0)

#define SURE_ABORT_ON_FALSE(cond, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_FALSE(cond))) { \
            SURE_ABORT(msg, ##__VA_ARGS__); \
        } \
    } while (0)

#define SURE_ABORT_ON_TRUE__NO_LOG(cond) do { \
        if (UNLIKELY_(SURE_IS_TRUE(cond))) { \
            SURE_ABORT(); \
        } \
    } while (0)

#define SURE_ABORT_ON_FALSE__NO_LOG(cond) do { \
        if (UNLIKELY_(SURE_IS_FALSE(cond))) { \
            SURE_ABORT(); \
        } \
    } while (0)


#define SURE_RET_ON_TRUE__LOG_ERR(cond, ret, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_TRUE(cond))) { \
            log_err_(msg, ##__VA_ARGS__); \
            return (ret); \
        } \
    } while (0)

#define SURE_RET_ON_FALSE__LOG_ERR(cond, ret, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_FALSE(cond))) { \
            log_err_(msg, ##__VA_ARGS__); \
            return (ret); \
        } \
    } while (0)

#define SURE_RET_ON_TRUE__LOG_WRN(cond, ret, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_TRUE(cond))) { \
            log_warn_(msg, ##__VA_ARGS__); \
            return (ret); \
        } \
    } while (0)

#define SURE_RET_ON_FALSE__LOG_WRN(cond, ret, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_FALSE(cond))) { \
            log_warn_(msg, ##__VA_ARGS__); \
            return (ret); \
        } \
    } while (0)

#define SURE_RET_ON_TRUE__NO_LOG(cond, ret) do { \
        if (UNLIKELY_(SURE_IS_TRUE(cond))) \
            return (ret); \
    } while (0)

#define SURE_RET_ON_FALSE__NO_LOG(cond, ret) do { \
        if (UNLIKELY_(SURE_IS_FALSE(cond))) \
            return (ret); \
    } while (0)

#define SURE_RET_VOID_IF_TRUE__LOG_ERR(cond, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_TRUE(cond))) { \
            log_err_(msg, ##__VA_ARGS__); \
            return; \
        } \
    } while (0)

#define SURE_RET_VOID_IF_FALSE__LOG_ERR(cond, ret, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_FALSE(cond))) { \
            log_err_(msg, ##__VA_ARGS__); \
            return; \
        } \
    } while (0)

#define SURE_RET_VOID_IF_TRUE__LOG_WRN(cond, ret, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_TRUE(cond))) { \
            log_warn_(msg, ##__VA_ARGS__); \
            return; \
        } \
    } while (0)

#define SURE_RET_VOID_IF_FALSE__LOG_WRN(cond, ret, msg, ...) do { \
        if (UNLIKELY_(SURE_IS_FALSE(cond))) { \
            log_warn_(msg, ##__VA_ARGS__); \
            return; \
        } \
    } while (0)

#define SURE_RET_VOID_IF_TRUE__NO_LOG(cond, ret) do { \
        if (UNLIKELY_(SURE_IS_TRUE(cond))) \
            return; \
    } while (0)

#define SURE_RET_VOID_IF_FALSE__NO_LOG(cond, ret) do { \
        if (UNLIKELY_(SURE_IS_FALSE(cond))) \
            return; \
    } while (0)


#define SURE_ABORT_ON_INV_PTR(ptr, msg, ...) SURE_ABORT_ON_FALSE(SURE_IS_VALID_PTR(ptr), false, msg, ##__VA_ARGS__)
#define SURE_ABORT_ON_INV_PTR__NO_LOG(ptr)   SURE_ABORT_ON_FALSE__NO_LOG(SURE_IS_VALID_PTR(ptr), false)

#define SURE_RET_BOOL_ON_FALSE(cond, msg, ...)          SURE_RET_ON_FALSE__LOG_ERR(cond, false, msg, ##__VA_ARGS__)
#define SURE_RET_BOOL_ON_FALSE__LOG_WRN(cond, msg, ...) SURE_RET_ON_FALSE__LOG_WRN(cond, false, msg, ##__VA_ARGS__)
#define SURE_RET_BOOL_ON_FALSE__NO_LOG(cond)            SURE_RET_ON_FALSE__NO_LOG(cond, false)

#define SURE_RET_BOOL_ON_INV_PTR(ptr, msg, ...)          SURE_RET_ON_TRUE__LOG_ERR(SURE_IS_INV_PTR(ptr), false, msg, ##__VA_ARGS__)
#define SURE_RET_BOOL_ON_INV_PTR__LOG_WRN(ptr, msg, ...) SURE_RET_ON_TRUE__LOG_WRN(SURE_IS_INV_PTR(ptr), false, msg, ##__VA_ARGS__)
#define SURE_RET_BOOL_ON_INV_PTR__NO_LOG(ptr)            SURE_RET_ON_TRUE__NO_LOG(SURE_IS_INV_PTR(ptr), false)

#define SURE_RET_NUM_ON_FALSE(cond, msg, ...)          SURE_RET_ON_FALSE__LOG_ERR(cond, 0, msg, ##__VA_ARGS__)
#define SURE_RET_NUM_ON_FALSE__LOG_WRN(cond, msg, ...) SURE_RET_ON_FALSE__LOG_WRN(cond, 0, msg, ##__VA_ARGS__)
#define SURE_RET_NUM_ON_FALSE__NO_LOG(cond)            SURE_RET_ON_FALSE__NO_LOG(cond, 0)

#define SURE_RET_NUM_ON_INV_PTR(ptr, msg, ...)          SURE_RET_ON_FALSE__LOG_ERR(SURE_IS_VALID_PTR(ptr), 0, msg, ##__VA_ARGS__)
#define SURE_RET_NUM_ON_INV_PTR__LOG_WRN(ptr, msg, ...) SURE_RET_ON_FALSE__LOG_WRN(SURE_IS_VALID_PTR(ptr), 0, msg, ##__VA_ARGS__)
#define SURE_RET_NUM_ON_INV_PTR__NO_LOG(ptr)            SURE_RET_ON_FALSE__NO_LOG(SURE_IS_VALID_PTR(ptr), 0)

#define SURE_RET_VOID_ON_FALSE(cond, msg, ...)          SURE_RET_ON_FALSE__LOG_ERR(cond, 0, msg, ##__VA_ARGS__)
#define SURE_RET_VOID_ON_FALSE__LOG_WRN(cond, msg, ...) SURE_RET_ON_FALSE__LOG_WRN(cond, 0, msg, ##__VA_ARGS__)
#define SURE_RET_VOID_ON_FALSE__NO_LOG(cond)            SURE_RET_ON_FALSE__NO_LOG(cond, 0)

#define SURE_RET_VOID_ON_INV_PTR(ptr, msg, ...)          SURE_RET_ON_FALSE__LOG_ERR(SURE_IS_VALID_PTR(ptr), 0, msg, ##__VA_ARGS__)
#define SURE_RET_VOID_ON_INV_PTR__LOG_WRN(ptr, msg, ...) SURE_RET_ON_FALSE__LOG_WRN(SURE_IS_VALID_PTR(ptr), 0, msg, ##__VA_ARGS__)
#define SURE_RET_VOID_ON_INV_PTR__NO_LOG(ptr)            SURE_RET_ON_FALSE__NO_LOG(SURE_IS_VALID_PTR(ptr), 0)

#endif // SURE_COMMON_BASE_ASSERT_HPP_
