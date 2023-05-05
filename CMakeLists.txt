cmake_minimum_required(VERSION 3.24)
project(constexpr_brainfuck)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-fconstexpr-steps=1271242)

add_executable(constexpr_brainfuck main.cpp)
