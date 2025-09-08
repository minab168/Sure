

#include "../Logging/Log.hpp"
#include "gtest/gtest.h"


TEST(logging, log_messages) {
    log_fat_("This is fatal message!");
    log_err_("This is an error message!");
    log_warn_("This is a warning message!");
    log_info_("This is an info message!");
    log_note_("This is a note message!");
    log_debug_("This is a debug message!");
}
