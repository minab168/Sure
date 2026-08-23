#pragma once


#include <stdlib.h>
#include "Logging/Inc.hpp"


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
