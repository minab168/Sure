# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


add_library(Utils
    INTERFACE
    Inc.hpp
    Check.hpp
    Abort.hpp
)


add_subdirectory(Logging)
add_subdirectory(Tests)