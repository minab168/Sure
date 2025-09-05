# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


add_library(Types
    INTERFACE
    Clone.hpp
    Inc.hpp
    Primitives/Limits.hpp
    Option.hpp
    Variant.hpp
    Result.hpp
    Primitives/Inc.hpp
    Primitives/Size.hpp
)


add_subdirectory(Tests)