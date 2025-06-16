// src/machine.hpp
#pragma once

#include "instruction.hpp"
#include <array>
#include <vector>
#include <cstdint>

class RiscMachine {
public:
    RiscMachine(size_t memory_size = 1024);

    void loadProgram(const std::vector<Instruction>& program);
    void run();
    void reset();
    void setMemoryValue(uint32_t address, uint32_t value);
    uint32_t getMemoryValue(uint32_t address) const;

private:
    void execute(const Instruction& instr);

    std::array<uint32_t, 8> registers{};  // R0–R7
    bool zero_flag = false;
    std::vector<Instruction> memory;     // memory stores instructions
    uint32_t pc = 0;                     // program counter
};
