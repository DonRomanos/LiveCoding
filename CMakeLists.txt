cmake_minimum_required(VERSION 3.22)

project(LiveCoding)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Lambdas)
add_subdirectory(StateMachine)
add_subdirectory(CrtpAndMixins)
