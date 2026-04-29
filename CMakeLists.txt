cmake_minimum_required(VERSION 3.23)
project(debug-plus)

# append homebrew path to prefix search
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt")

# find LLVM API
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in:${LLVM_DIR}")

# Set compile flags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include LLVM
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_compile_options(-Wall -Wextra -Werror -Wconversion -pedantic -Wsign-conversion -Wfloat-conversion)

# build
add_executable(debugger main.cpp)

# find libraries corresponding to the LLVM components we want to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# link against llvm libs
target_link_libraries(debugger ${llvm_libs})
