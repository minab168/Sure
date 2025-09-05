# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


set(SRC
    Option.cpp
)

add_executable(TestTypes ${SRC})

target_link_libraries(TestTypes
    gtest
    gtest_main
)


add_test(NAME TypesTests COMMAND TestTypes)