cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(sltl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(sltl)
add_subdirectory(sltl_cmd)
add_subdirectory(sltl_test)
