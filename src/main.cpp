// src/main.cpp
#include "machine.hpp"
#include <iostream>
#include <vector>
#include <cstdint>
#include "instruction.hpp"

int main() {
    RiscMachine machine(256 /*program*/, 256 /*data*/);

    // Define addresses for input and output
    // Set memory addresses
    uint32_t input_addr = 100;
    uint32_t const_one_addr = 101;
    uint32_t result_addr = 102;

    // Initialize input
    machine.setMemoryValue(input_addr, 6);        // n = 6 (compute 6!)
    machine.setMemoryValue(const_one_addr, 1);    // constant 1

    // Define factorial program
    std::vector<Instruction> factorial_program = {
        {Opcode::LOAD, 0, const_one_addr, 0},   // R0 = 1 (result)
        {Opcode::LOAD, 1, input_addr, 0},       // R1 = n
        {Opcode::LOAD, 2, const_one_addr, 0},   // R2 = 1

        // loop_start
        {Opcode::CMP, 0, 1, 2},                // if R1 == 1
        {Opcode::JMP, 8, 1, 0},                // if ZF == 1 → skip MUL/SUB

        {Opcode::MUL, 0, 0, 1},                 // result *= i
        {Opcode::SUB, 1, 1, 2},                 // i--
        {Opcode::JMP, 3, 0, 0},                 // jump to loop_start

        {Opcode::STORE, result_addr, 0, 0},     // RAM[result_addr] = result
        {Opcode::HALT, 0, 0, 0}
    };

    // Load initial memory values
    // We'll use instruction memory for both program and data
    machine.loadProgram(factorial_program);

    // Run the program
    machine.run();

    // Display result
    std::cout << "Factorial of n = "
              << machine.getMemoryValue(input_addr)
              << " is "
              << machine.getMemoryValue(result_addr)
              << std::endl;

    return 0;
}
