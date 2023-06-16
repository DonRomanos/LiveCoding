set(CMAKE_CXX_STANDARD 17)
include(ExternalProject)
include(FetchContent)

set(EXTERNAL_DIR ${CMAKE_CURRENT_BINARY_DIR}/external)

FetchContent_Declare(
  sml
  GIT_REPOSITORY https://github.com/boost-ext/sml.git
  SOURCE_DIR ${EXTERNAL_DIR}/sml
)
FetchContent_MakeAvailable(sml)

ExternalProject_Add(hana
    GIT_REPOSITORY https://github.com/boostorg/hana.git
    DOWNLOAD_DIR ${EXTERNAL_DIR}/hana
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DIR}/hana
)

add_executable(sml_state_machine main.cpp)

add_dependencies(sml_state_machine hana)

message(STATUS "INLUCDE: ${EXTERNAL_DIR}/sml/include")

target_include_directories(sml_state_machine PRIVATE ${EXTERNAL_DIR}/hana/include ${EXTERNAL_DIR}/sml/include)
