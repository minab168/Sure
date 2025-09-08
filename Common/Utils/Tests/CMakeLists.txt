# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


set(SRC
    Logging.cpp
    Check.cpp
)

add_executable(TestUtils ${SRC})

target_link_libraries(TestUtils
    gtest
    gtest_main
)


add_test(NAME UtilssTests COMMAND TestUtils)