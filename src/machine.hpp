// machine.hpp
#pragma once

#include "instruction.hpp"
#include <array>
#include <vector>
#include <cstdint>

struct StatusRegister {
    uint32_t ZF : 1;        // Zero flag compare
    uint32_t CF : 1;        // Carry flag
    uint32_t NF : 1;        // Negative flag
    uint32_t OF : 1;        // Overflow flag
    uint32_t DF : 1;        // Division by zero flag
    uint32_t reserved : 28; // Reserved bits
};

class RiscMachine {
public:
    RiscMachine(size_t program_size = 256, size_t data_size = 1024);

    void loadProgram(const std::vector<Instruction>& program);
    void run();
    void reset();

    void setMemoryValue(uint32_t address, uint32_t value);
    uint32_t getMemoryValue(uint32_t address) const;
    StatusRegister getStatusRegister() const;

private:
    void execute(const Instruction& instr);

    std::array<uint32_t, 8> data_registers{};  // R0–R7
    StatusRegister status_register{};

    uint32_t pc = 0;  // program counter

    std::vector<Instruction> program_memory;
    std::vector<uint32_t> data_memory;
};