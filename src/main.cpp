// src/main.cpp
#include "machine.hpp"
#include <iostream>
#include <vector>
#include <cstdint>
#include "instruction.hpp"

int main() {
    RiscMachine machine;

    // Example: compute 5 + 7 and store result in memory[0]
    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},      // R0 = memory[100] (5)
        {Opcode::LOAD, 1, 101, 0},      // R1 = memory[101] (7)
        {Opcode::ADD,  2, 0, 1},        // R2 = R0 + R1
        {Opcode::STORE, 0, 2, 0},       // memory[0] = R2
        {Opcode::HALT, 0, 0, 0}         // stop
    };

    // Load initial memory values
    // We'll use instruction memory for both program and data
    machine.loadProgram(program);

    // Manually initialize memory[100] = 5 and memory[101] = 7
    machine.setMemoryValue(100, 5);
    machine.setMemoryValue(101, 7);

    // Run the program
    machine.run();

    // Show result stored in memory[0]
    uint32_t result = machine.getMemoryValue(0);
    std::cout << "Result of 5 + 7 stored in memory[0]: " << result << std::endl;

    return 0;
}
