cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(Variables LANGUAGES CXX)

add_executable(cross_compiling main.cpp)

if(CMAKE_CROSSCOMPILING)
    message(STATUS "We are cross compiling for ${CMAKE_SYSTEM_NAME}")
endif()

#find_package()
