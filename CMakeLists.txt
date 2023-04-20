cmake_minimum_required(VERSION 3.0)

project(sdl-game VERSION 0.1.0
  DESCRIPTION "A game"
  LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

include_directories(
  "${CMAKE_SOURCE_DIR}/modules"
  "${CMAKE_SOURCE_DIR}/src"
  "${PROJECT_BINARY_DIR}/src"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_BINARY_DIR}/src/config.h"
)

enable_testing()
add_subdirectory(src)
add_subdirectory(test EXCLUDE_FROM_ALL)
