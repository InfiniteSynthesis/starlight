
cmake_minimum_required(VERSION 2.6)

project(layout_test)

set(CMAKE_CXX_FLAGS
    "-std=c++14 -Wall -Wextra -Wno-unused-parameter -DTESTING"
)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

include_directories(${CMAKE_SOURCE_DIR}/../Core)
include_directories(${CMAKE_SOURCE_DIR}/../Core/third_party/googletest/include
                    ${CMAKE_SOURCE_DIR}/../Core/third_party/googletest)
include_directories(${CMAKE_SOURCE_DIR}/../layout_test/cxx)

add_library(layout_test
    ${CMAKE_SOURCE_DIR}/../Core/third_party/googletest/src/gtest-all.cc
    ${CMAKE_SOURCE_DIR}/../Core/base/length_utils.h
    ${CMAKE_SOURCE_DIR}/../Core/base/length.h
    ${CMAKE_SOURCE_DIR}/../Core/base/string_utils.cc
    ${CMAKE_SOURCE_DIR}/../Core/base/string_utils.h
    ${CMAKE_SOURCE_DIR}/../Core/layout/flex_layout.cc
    ${CMAKE_SOURCE_DIR}/../Core/layout/flex_layout.h
    ${CMAKE_SOURCE_DIR}/../Core/layout/layout_algorithm.h
    ${CMAKE_SOURCE_DIR}/../Core/layout/layout_enum.h
    ${CMAKE_SOURCE_DIR}/../Core/layout/layout_node.cc
    ${CMAKE_SOURCE_DIR}/../Core/layout/layout_node.h
    ${CMAKE_SOURCE_DIR}/../Core/layout/mock_layout_host.h
    ${CMAKE_SOURCE_DIR}/../Core/layout/style.cc
    ${CMAKE_SOURCE_DIR}/../Core/layout/style.h
    )


add_executable(layout_test_execute
    src/main.cpp
    )

target_link_libraries(layout_test_execute
    layout_test
    )

set_target_properties(layout_test_execute
    PROPERTIES OUTPUT_NAME layout_test
    )
