cmake_minimum_required(VERSION 3.2)
project(lab1)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

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
