cmake_minimum_required(VERSION 3.2)
project(lab1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(SOURCE_FILES utils.h count.h)
add_executable(lab1 main.cpp ${SOURCE_FILES})

################################
# GTest
################################
ADD_SUBDIRECTORY(googletest-release-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable(runTest test.cpp ${SOURCE_FILES})
# Link test executable against gtest & gtest_main
target_link_libraries(runTest gtest gtest_main)
add_test( runTest runTest )
