#pragma once


#include <stdio.h>
#include <time.h>

#include "Color.hpp"
#include "../../Base/Lang.hpp"
#include "../../Types/Primitives/Inc.hpp"


#define LOG_COLOR_RESET   COLOR_RESET
#define LOG_COLOR_FATAL   STYLE_BOLD COLOR_BG_RED
#define LOG_COLOR_ERROR   COLOR_RED
#define LOG_COLOR_WARN    COLOR_YELLOW
#define LOG_COLOR_INFO    COLOR_GREEN
#define LOG_COLOR_NOTE    COLOR_CYAN
#define LOG_COLOR_DEBUG   COLOR_MAGENTA


static constexpr void get_timestamp(Char* buf, U64 size) {
    time_t t = time(NULL_PTR);
    struct tm tm_info = {};

    #if OS_WINDOWS_ == 1
        localtime_s(&tm_info, &t);
    #else
        localtime_r(&t, &tm_info);
    #endif

    strftime(buf, size, "%Y-%m-%d %H:%M:%S", &tm_info);
}


#define log_timestamp_(buf_) get_timestamp(buf_, sizeof(buf_))


#define log_print_(level_, color_, fmt_, ...) do {                \
        Char ts[20];                                            \
        log_timestamp_(ts);                                    \
        printf("%s[%s] %s: " fmt_ "%s\n", color_, ts, level_, ##__VA_ARGS__, LOG_COLOR_RESET); \
    } while(false)


#define log_fat_(fmt_, ...)  log_print_("FATAL",   LOG_COLOR_FATAL, fmt_, ##__VA_ARGS__)
#define log_err_(fmt_, ...)  log_print_("ERROR",   LOG_COLOR_ERROR, fmt_, ##__VA_ARGS__)
#define log_warn_(fmt_, ...) log_print_("WARNING", LOG_COLOR_WARN,  fmt_, ##__VA_ARGS__)
#define log_info_(fmt_, ...) log_print_("INFO",    LOG_COLOR_INFO,  fmt_, ##__VA_ARGS__)
#define log_note_(fmt_, ...) log_print_("NOTE",    LOG_COLOR_NOTE,  fmt_, ##__VA_ARGS__)

#ifndef NO_DEBUG_
#  define log_debug_(fmt_, ...) log_print_("DEBUG",   LOG_COLOR_DEBUG, fmt_, ##__VA_ARGS__)
#else
#  define log_debug_(fmt_, ...) do {} while(false)
#endif