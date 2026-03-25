include(FetchContent)

enable_testing()

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(gg_test_memory memory.cpp)
target_link_libraries(gg_test_memory PRIVATE ggemu gtest_main)
set_property(TARGET gg_test_memory PROPERTY CXX_STANDARD 23)

add_executable(gg_test_register register.cpp)
target_link_libraries(gg_test_register PRIVATE ggemu gtest_main)
set_property(TARGET gg_test_register PROPERTY CXX_STANDARD 23)

include(GoogleTest)
gtest_discover_tests(gg_test_memory)
gtest_discover_tests(gg_test_register)