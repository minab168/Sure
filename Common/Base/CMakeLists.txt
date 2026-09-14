# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


add_library(Base
    INTERFACE
    Interface/Compiler.hpp
    Interface/Inc.hpp
    Interface/Lang.hpp
    Interface/Platform.hpp
    Interface/Memory.hpp
    Interface/Runtime.hpp
)