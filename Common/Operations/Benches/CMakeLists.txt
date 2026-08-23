# ──────────────────────────────────────────────
# Project   : Sure
# File      : CMakeLists.txt
# Author    : Emad Redwan
# Created   : 9/4/25
# License   : MIT
# ──────────────────────────────────────────────


set(SRC
    BitField.cpp
)

add_executable(BenchOperations ${SRC})

if (MSVC)
    target_compile_options(BenchOperations PRIVATE /O2 /Ob3)
else ()
    target_compile_options(BenchOperations PRIVATE -O3)
endif ()

target_compile_definitions(BenchOperations PRIVATE NDEBUG)