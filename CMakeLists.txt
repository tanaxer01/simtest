cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SchedulerTesting)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(SimGrid 3.27 REQUIRED)

include_directories(${SimGrid_Include_DIR})

add_executable(test_01 test_01.cpp test_01.hpp)
target_link_libraries(test_01 ${SimGrid_LIBRARY})

add_executable(test_02 test_02.cpp)
target_link_libraries(test_02 ${SimGrid_LIBRARY})