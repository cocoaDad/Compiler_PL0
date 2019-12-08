cmake_minimum_required(VERSION 3.14)
project(Compiler_PL0)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler_PL0 lexPL0.cpp lexPL0.h SLR1PL0.cpp SLR1PL0.h main.cpp)