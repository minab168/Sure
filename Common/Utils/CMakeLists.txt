# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


add_library(Utils
    INTERFACE
    Abort.hpp
    Assert.hpp
    Memory.hpp
    Check.hpp
    Inc.hpp
)


add_subdirectory(Collections)
add_subdirectory(Location)
add_subdirectory(Logging)
add_subdirectory(Tests)