cmake_minimum_required(VERSION 3.28)

# Project metadata
project(TapeSorter VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Ninja colored output
set(CMAKE_COLOR_DIAGNOSTICS ON)

# --- GTest Setup ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For MSVC
FetchContent_MakeAvailable(googletest)

# --- Source Files ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h
)

# Make the library so both app and tests can reuse logic
add_library(tape_core ${SOURCES})
target_include_directories(tape_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Final executable
add_executable(tape_sorter src/main.cpp)
target_link_libraries(tape_sorter PRIVATE tape_core)

# --- Tests ---
enable_testing()

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

add_executable(test_all ${TEST_SOURCES})
target_link_libraries(test_all PRIVATE tape_core gtest_main)
target_include_directories(test_all PRIVATE ${CMAKE_SOURCE_DIR}/src)

include(GoogleTest)
gtest_discover_tests(test_all)