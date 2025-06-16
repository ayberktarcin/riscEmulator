cmake_minimum_required(VERSION 3.10)

project(RiscEmulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
add_executable(RiscEmulator
    src/main.cpp
    src/machine.cpp
)