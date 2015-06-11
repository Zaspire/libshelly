cmake_minimum_required (VERSION 3.0)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project (libshelly)
set (libshelly_VERSION_MAJOR 1)
set (libshelly_VERSION_MINOR 1)

set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -Wextra")

set(shelly_headers
  include/common.hpp
  include/shelly.hpp
  include/avl_tree.hpp
  include/big_int.hpp
)

enable_testing()

add_executable(test_common
  tests/common.cpp
  ${shelly_headers}
)
target_link_libraries(test_common
  ${GTEST_LIBS_DIR}/libgtest.a
  pthread
  ${GTEST_LIBS_DIR}/libgtest_main.a
)
add_dependencies(test_common googletest)

add_executable(test_bigint
  tests/big_int.cpp
)
target_link_libraries(test_bigint
  ${GTEST_LIBS_DIR}/libgtest.a
  pthread
  ${GTEST_LIBS_DIR}/libgtest_main.a
)
add_dependencies(test_bigint googletest)

add_test(common test_common)
add_test(bigint test_bigint)
