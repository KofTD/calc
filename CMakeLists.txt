cmake_minimum_required(VERSION 3.23)
project(calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(calculator main.cpp operand.cpp operand.h
        notation_operations.cpp
        notation_operations.h
        variables.cpp
        variables.h
        calculations.cpp
        calculations.h)
