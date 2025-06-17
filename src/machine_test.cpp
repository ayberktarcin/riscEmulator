// src/machine_test.cpp
#include "machine.hpp"
#include <iostream>
#include <cassert>

void test_addition() {
    RiscMachine machine;
    machine.setMemoryValue(100, 5);
    machine.setMemoryValue(101, 7);

    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 100, 0},    // R0 = 5
        {Opcode::LOAD, 1, 101, 0},    // R1 = 7
        {Opcode::ADD, 2, 0, 1},       // R2 = R0 + R1
        {Opcode::STORE, 102, 2, 0},   // memory[102] = R2
        {Opcode::HALT, 0, 0, 0}
    };

    machine.loadProgram(program);
    machine.run();

    uint32_t result = machine.getMemoryValue(102);
    assert(result == 12);
    std::cout << "✅ test_addition passed.\n";
}

void test_multiplication() {
    RiscMachine machine;
    machine.setMemoryValue(200, 3);
    machine.setMemoryValue(201, 4);

    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 200, 0},    // R0 = 3
        {Opcode::LOAD, 1, 201, 0},    // R1 = 4
        {Opcode::MUL, 2, 0, 1},       // R2 = R0 * R1
        {Opcode::STORE, 202, 2, 0},   // memory[202] = R2
        {Opcode::HALT, 0, 0, 0}
    };

    machine.loadProgram(program);
    machine.run();

    uint32_t result = machine.getMemoryValue(202);
    assert(result == 12);
    std::cout << "✅ test_multiplication passed.\n";
}

void test_zero_flag_and_cmp() {
    RiscMachine machine;
    machine.setMemoryValue(300, 5);

    std::vector<Instruction> program = {
        {Opcode::LOAD, 0, 300, 0},    // R0 = 5
        {Opcode::LOAD, 1, 300, 0},    // R1 = 5
        {Opcode::CMP, 0, 0, 1},       // Compare R0 and R1
        {Opcode::HALT, 0, 0, 0}
    };

    machine.loadProgram(program);
    machine.run();

    // Cannot directly access zero_flag, but at least we make sure program completes
    std::cout << "✅ test_zero_flag_and_cmp executed (manual check if needed).\n";
}

int main() {
    test_addition();
    test_multiplication();
    test_zero_flag_and_cmp();
    std::cout << "✅ All tests completed.\n";
    return 0;
}
