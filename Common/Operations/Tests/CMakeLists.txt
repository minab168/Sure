# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


set(SRC
    Bitwise.cpp
    BitField.cpp
)

add_executable(TestOperators ${SRC})

target_link_libraries(TestOperators
    gtest
    gtest_main
)


add_test(NAME OperatorsTest COMMAND TestOperators)