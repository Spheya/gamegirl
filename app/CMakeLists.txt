add_executable(gamegirl main.cpp)
target_link_libraries(gamegirl PRIVATE ggemu)

set_property(TARGET gamegirl PROPERTY CXX_STANDARD 23)
target_compile_definitions(gamegirl PRIVATE $<$<CONFIG:Release>:NDEBUG>)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(gamegirl PRIVATE -Wall -Wpedantic)
endif()
