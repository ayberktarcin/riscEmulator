cmake_minimum_required(VERSION 3.10)

project(RiscEmulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add source files
add_executable(RiscEmulator
    src/machine.cpp
    src/main.cpp
    src/algorithms.cpp
)

add_executable(MachineTest
    tests/machine_gtest.cpp
    src/machine.cpp
    src/algorithms.cpp
)
enable_testing()
target_link_libraries(MachineTest gtest gtest_main pthread)

add_test(NAME MachineTest COMMAND MachineTest)