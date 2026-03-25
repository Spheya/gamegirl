add_library(ggemu 
    "ggemu/logger.cpp" "ggemu/logger.hpp" "ggemu/memory.cpp" "ggemu/memory.hpp" "ggemu/register.hpp" "ggemu/types.hpp"
)
target_include_directories(ggemu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_property(TARGET ggemu PROPERTY CXX_STANDARD 23)
target_compile_definitions(ggemu PRIVATE $<$<CONFIG:Release>:NDEBUG>)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ggemu PRIVATE -Wall -Wpedantic)
endif()
