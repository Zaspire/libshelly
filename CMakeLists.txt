cmake_minimum_required (VERSION 3.0)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project (libshelly)
set (libshelly_VERSION_MAJOR 1)
set (libshelly_VERSION_MINOR 1)

set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/shelly)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -Wextra")

set(shelly_headers
  shelly/common.hpp
  shelly/shelly.hpp
  shelly/avl_tree.hpp
  shelly/big_int.hpp
  shelly/stream.hpp
  shelly/fenwick_tree.hpp
  shelly/string_utils.hpp
  shelly/algorithm.hpp
)

enable_testing()

add_executable(test_all
  tests/big_int.cpp
  tests/avl_tree.cpp
  tests/math.cpp
  tests/common.cpp
  tests/string_utils.cpp
  tests/stream.cpp
  tests/algorithm.cpp
  ${shelly_headers}
)
target_link_libraries(test_all
  ${GTEST_LIBS_DIR}/libgtest.a
  pthread
  ${GTEST_LIBS_DIR}/libgtest_main.a
)
add_dependencies(test_all googletest)

add_test(all test_all)
