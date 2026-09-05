#pragma once


#include <stdlib.h>
#include "Logging/Inc.hpp"
#include "Location/Inc.hpp"


#define ABORT(fmt, ...) do { \
        log_fat_("Process aborted at %s:%d in %s()", FILE_, LINE_, FUNCTION_); \
        if (fmt) log_err_(fmt, ##__VA_ARGS__); \
        ::abort(); \
    } while(0)


// deprecated
#define abort_(fmt_, ...) do { \
        log_fat_("Process aborted at %s:%d in %s()", __FILE__, __LINE__, __func__); \
        if (fmt_) log_err_(fmt_, ##__VA_ARGS__); \
        ::abort(); \
    } while(0)


#define exit_with_(code_, fmt_, ...) do { \
        if (fmt_) log_err_(fmt_, ##__VA_ARGS__); \
        ::exit(code_); \
    } while(0)


#define fail_(ret_val_, fmt_, ...) do { \
        if (fmt_) log_err_(fmt_, ##__VA_ARGS__); \
        return ret_val_; \
    } while(0)
