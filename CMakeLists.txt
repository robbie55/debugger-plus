cmake_minimum_required(VERSION 3.23)
project(debug-plus)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COM)

add_compile_options(-Wall -Wextra -Werror -Wconversion -pedantic -Wsign-conversion -Wfloat-conversion)

add_executable(debugger
  main.cpp
)
