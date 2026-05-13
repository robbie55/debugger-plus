cmake_minimum_required(VERSION 3.23)
project(debug-plus)

# prepend homebrew path to prefix search, use llvm directly
# list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt")
list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/llvm")

# find LLVM API
find_package(LLVM REQUIRED CONFIG)

find_library(LLDB_LIBRARY
  NAMES lldb
  PATHS "/opt/homebrew/opt/llvm/lib"
  REQUIRED
)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in:${LLVM_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})

# Add compile directives as a warning library
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE
  -Wall -Wextra -Werror -Wconversion -pedantic -Wsign-conversion -Wfloat-conversion
)

enable_testing()

# add core lib
add_subdirectory(src/debugplus)

# add tests dir
add_subdirectory(tests)

add_executable(debugger src/main.cpp)
target_link_libraries(debugger PRIVATE debugplus_core)

# link against project_warnings
target_link_libraries(debugger PRIVATE project_warnings)
