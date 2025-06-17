cmake_minimum_required(VERSION 3.10)

project(RiscEmulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
add_executable(RiscEmulator
    src/machine.cpp
    src/machine_test.cpp    #src/main.cpp
)