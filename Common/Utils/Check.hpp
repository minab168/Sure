#pragma once


#include "Logging/Inc.hpp"
#include "Abort.hpp"


#define CHECK_ABORT(bool_expr, ...)    do { \
    if ((bool_expr)) { \
    ABORT(__VA_ARGS__); \
} \
} while(0)


#define check_result_(res_expr_, ...)    do { \
        auto _res = (res_expr_); \
        if (!_res.is_ok) { \
            if (sizeof((char[]){__VA_ARGS__})>1) log_err_(__VA_ARGS__); \
            return _res; \
        } \
    } while(0)


#define check_option_(opt_expr_, ...)    do { \
        auto _opt = do_move(opt_expr_); \
        if (!_opt.is_none())) { \
            if (sizeof((char[]){__VA_ARGS__})>1) log_err_(__VA_ARGS__); \
            return do_move(_opt); \
        } \
    } while(0)


#define check_option_ret_void_(opt_expr_, ...)    do { \
        auto _opt = do_move(opt_expr_); \
        if (_opt.is_none()) { \
            if (sizeof((char[]){__VA_ARGS__})>1) log_err_(__VA_ARGS__); \
            return; \
        } \
    } while(0)
