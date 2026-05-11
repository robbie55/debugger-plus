cmake_minimum_required(VERSION 3.23)
project(debug-plus)

# append homebrew path to prefix search
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt")

# find LLVM API
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in:${LLVM_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})

# find libraries corresponding to the LLVM components we want to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Add compile directives as a warning library
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE
  -Wall -Wextra -Werror -Wconversion -pedantic -Wsign-conversion -Wfloat-conversion
)

enable_testing()

# add tests dir
add_subdirectory(./tests)

# build
add_executable(debugger main.cpp)

# link against llvm libs
target_link_libraries(debugger PRIVATE ${llvm_libs})
target_include_directories(debugger PRIVATE ${LLVM_INCLUDE_DIRS})

# link against project_warnings
target_link_libraries(debugger PRIVATE project_warnings)

# target compile directives
target_compile_features(debugger PRIVATE cxx_std_20)
target_compile_definitions(debugger PRIVATE ${LLVM_DEFINITIONS_LIST})
