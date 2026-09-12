# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


add_library(Types
    INTERFACE
    Array.hpp
    Inc.hpp
    Option.hpp
    Primitives/Inc.hpp
    Primitives/Limits.hpp
    Primitives/Size.hpp
    Result.hpp
    UPtr.hpp
    Variant.hpp
)


add_subdirectory(Tests)