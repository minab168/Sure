# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


set(SRC
    MMIO.cpp
)

add_executable(TestHardware ${SRC})

target_link_libraries(TestHardware
    gtest
    gtest_main
)


add_test(NAME HardwareTest COMMAND TestHardware)