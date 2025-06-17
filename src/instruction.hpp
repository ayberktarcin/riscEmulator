// src/instruction.hpp
#pragma once

#include <cstdint>

enum class Opcode {
    HALT,   // Stop execution
    LOAD,   // Load value from memory to register
    STORE,  // Store value from register to memory
    ADD,    // Add two registers and store result in destination
    SUB,    // Subtract two registers and store result in destination
    CMP,    // Compare two registers and set flags
    JMP,     // Jump to specified address
    MUL,    // Multiply two registers and store result in destination
    DIV    // Divide two registers and store result in destination
};

struct Instruction {
    Opcode opcode;
    uint32_t dst = 0;
    uint32_t src1 = 0;
    uint32_t src2 = 0;
};
