// src/instruction.hpp
#pragma once

#include <cstdint>

enum class Opcode {
    HALT,
    LOAD,
    STORE,
    ADD,
    SUB,
    CMP,
    JMP
};

struct Instruction {
    Opcode opcode;
    uint8_t dst = 0;
    uint8_t src1 = 0;
    uint8_t src2 = 0;
};
