cmake_minimum_required(VERSION 3.5)
project(Challenge)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES gtest/[1.11] benchmark/[>1.4]
                      GENERATORS cmake_find_package)
                      
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter 
                    SETTINGS ${settings})

option(BUILD_TESTS "Will build the tests verifying your result" ON)
option(BUILD_REFERENCE "Will build a reference implementation as a library, 
                        which will also be tested and benchmarked. Cool for comparison." ON)
option(BUILD_BENCHMARK "This will enable the performance tests using google benchmark" ON)

add_library(${PROJECT_NAME} INTERFACE)
add_library(Challenge::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE src/)

target_sources(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/challenge.hpp)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_compile_options(${PROJECT_NAME} INTERFACE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion -Wno-gnu-zero-variadic-macro-arguments>
    $<$<CXX_COMPILER_ID:MSVC>:
        /WX /W4>
)

######################################################################
if(BUILD_REFERENCE)
    add_library(${PROJECT_NAME}_Reference INTERFACE)
    add_library(Challenge::${PROJECT_NAME}_Reference ALIAS ${PROJECT_NAME}_Reference)

    target_include_directories(${PROJECT_NAME}_Reference INTERFACE src/)

    target_sources(${PROJECT_NAME}_Reference INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/reference.hpp)

    target_compile_definitions(${PROJECT_NAME}_Reference INTERFACE BUILD_REFERENCE)

    target_compile_features(${PROJECT_NAME}_Reference INTERFACE cxx_std_20)

    target_compile_options(${PROJECT_NAME}_Reference INTERFACE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion -Wno-gnu-zero-variadic-macro-arguments>
        $<$<CXX_COMPILER_ID:MSVC>:
            /WX /W4>
    )
endif()

######################################################################
if( BUILD_TESTS)
    find_package(GTest REQUIRED)

    add_executable(${PROJECT_NAME}_Tests)
    
    target_sources(${PROJECT_NAME}_Tests PRIVATE 
        src/tests.cpp)

    target_include_directories(${PROJECT_NAME}_Tests PRIVATE
        src/)

    target_link_libraries(${PROJECT_NAME}_Tests PRIVATE
        Challenge::${PROJECT_NAME}
        GTest::GTest
        GTest::gtest_main
        $<$<TARGET_EXISTS:Challenge::${PROJECT_NAME}_Reference>:Challenge::${PROJECT_NAME}_Reference>)

    target_compile_options(${PROJECT_NAME}_Tests PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
            -Wno-gnu-zero-variadic-macro-arguments>
    )

    include(GoogleTest)
    enable_testing()
    gtest_discover_tests(${PROJECT_NAME}_Tests)
endif()

######################################################################
if( BUILD_BENCHMARK)
    find_package(benchmark REQUIRED)
    # Requires PThreads on Linux
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    add_executable(${PROJECT_NAME}_Benchmark)

    target_sources(${PROJECT_NAME}_Benchmark PRIVATE 
        src/benchmark.cpp)

    target_include_directories(${PROJECT_NAME}_Benchmark PRIVATE
        src/benchmark)

    target_link_libraries(${PROJECT_NAME}_Benchmark PRIVATE
        Challenge::${PROJECT_NAME}
        benchmark::benchmark
        Threads::Threads
        $<$<TARGET_EXISTS:Challenge::${PROJECT_NAME}_Reference>:Challenge::${PROJECT_NAME}_Reference>)

    add_custom_target(NAME ${PROJECT_NAME}_Benchmark
        COMMAND ${PROJECT_NAME}_Benchmark
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
