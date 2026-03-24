cmake_minimum_required(VERSION 3.20)

option(GAMEGIRL_BUILD_TESTS "Build unit tests" OFF)

project(gamegirl)

add_subdirectory(emulator)
add_subdirectory(app)

if(GAMEGIRL_BUILD_TESTS)
    add_subdirectory(tests)
endif()