set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_CXX_COMPILER clang++)
# Requires that you install the target package
set(CMAKE_CXX_COMPILER_TARGET arm-linux-gnueabihf)

# What should go here? In general: Everything that affects binary compartibility
# Ideally compile everything with the same settings (toolchain file).
set(CMAKE_CXX_STANDARD 23)

# What should not go in here, stuff that you don't want to propagate to all your dependencies, e.g. -Wall

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
