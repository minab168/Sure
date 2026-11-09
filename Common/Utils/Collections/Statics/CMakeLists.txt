# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


add_library(StaticCollections
    INTERFACE
    Inc.hpp
    RingBuf.hpp
    StaticVec.hpp
)


if(SURE_BUILD_TESTS)
    add_subdirectory(Tests)
endif()