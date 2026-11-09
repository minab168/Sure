add_library(Hardware INTERFACE
    Interface/Inc.hpp
)



if(SURE_BUILD_TESTS)
    add_subdirectory(Tests)
endif()