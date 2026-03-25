cmake_minimum_required(VERSION 3.20)

if(POLICY CMP0135)
  cmake_policy(PUSH)
  cmake_policy(SET CMP0135 NEW)
endif()

option(GAMEGIRL_BUILD_TESTS "Build unit tests" OFF)

project(gamegirl)

add_subdirectory(emulator)
add_subdirectory(app)

if(GAMEGIRL_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(POLICY CMP0135)
  cmake_policy(POP)
endif()
