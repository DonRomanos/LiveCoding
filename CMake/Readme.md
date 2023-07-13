# Things to show

## CMake variables

CMake has only one type which is string, meaning everything in CMake is a string. Lists are just strings concatenated with `;` so this "one;two" is a list in CMake.

Printing variables can be done with a helper:
```
include(CMakePrintHelpers)
cmake_print_variables(foo)
```

Debugging/Tracing can also be done through some command line flags:
```
cmake ../CMake --trace-expand . --debug-output --trace-source=../CMake/CMakeLists.txt
```
CMake will get a debug interface in version 3.27

Things that introduce scopes:
* add_subdirectory
* function
* block (since CMake 3.25)




* Linking - Public Private Interface Cyclic dependencies
* Directory structure
* Fetching content
* Custom targets and custom commands
* Scripting mode and environment variables
* CMakePresets
* Cross compiling / Toolchain files
* Installing / Creating a package
* CMake as package management
