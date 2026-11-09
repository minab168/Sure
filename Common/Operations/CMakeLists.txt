# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


add_library(Operations
    INTERFACE
    Bitwise.hpp
    BitField.hpp
    Alignment.hpp
    Inc.hpp
)


if(SURE_BUILD_TESTS)
    add_subdirectory(Tests)
    add_subdirectory(Benches)
endif()