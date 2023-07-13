cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(Variables LANGUAGES CXX)

include(CMakePrintHelpers)

# Variables

## Which values will we see for this variable?
set(foo x1 x2 x3)

cmake_print_variables(foo)

function(some_function)
    set(foo function)
endfunction()
some_function()
## How about now?
cmake_print_variables(foo)


macro(some_macro)
    set(foo macro)
endmacro()
some_macro()
## And now?
cmake_print_variables(foo)

# How about this?
set(cached cached CACHE STRING "A cache variable")
cmake_print_variables(cached)

set(cached set_without_cache_keyword)
cmake_print_variables(cached)

function(inside_function)
    set(cached "inside a function")
    cmake_print_variables(cached)
endfunction()
inside_function()
cmake_print_variables(cached)

# What if I comment this out now, what will the value be?
set(cached cached_force CACHE STRING "A cache variable" FORCE)
cmake_print_variables(cached)

list(APPEND cached appended)
cmake_print_variables(cached)
