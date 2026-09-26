# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


set(SRC
    StaticVec.cpp
)

add_executable(TestStaticCollections ${SRC})

target_link_libraries(TestStaticCollections
    gtest
    gtest_main
)


add_test(NAME StaticCollectionsTests COMMAND TestStaticCollections)