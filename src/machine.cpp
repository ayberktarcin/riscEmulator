// src/machine.cpp
#include "machine.hpp"
#include <iostream>

RiscMachine::RiscMachine(size_t memory_size) {
    memory.resize(memory_size);
}

void RiscMachine::loadProgram(const std::vector<Instruction>& program) {
    std::copy(program.begin(), program.end(), memory.begin());
    pc = 0;
    zero_flag = false;
    registers.fill(0);
}

void RiscMachine::run() {
    while (pc < memory.size()) {
        Instruction instr = memory[pc];
        pc++;
        execute(instr);

        if (instr.opcode == Opcode::HALT)
            break;
    }
}

void RiscMachine::reset() {
    pc = 0;
    zero_flag = false;
    registers.fill(0);
}

void RiscMachine::execute(const Instruction& instr) {
    switch (instr.opcode) {
        case Opcode::HALT:
            // nothing needed, loop will break
            break;

        case Opcode::LOAD:
            // dst = memory[src1]
            if (instr.src1 < memory.size() && instr.dst < registers.size())
                registers[instr.dst] = reinterpret_cast<uint32_t&>(memory[instr.src1]);
            break;

        case Opcode::STORE:
            // memory[dst] = registers[src1]
            if (instr.dst < memory.size() && instr.src1 < registers.size())
                reinterpret_cast<uint32_t&>(memory[instr.dst]) = registers[instr.src1];
            break;

        case Opcode::ADD:
            if (instr.dst < registers.size() && instr.src1 < registers.size() && instr.src2 < registers.size())
                registers[instr.dst] = registers[instr.src1] + registers[instr.src2];
            break;

        case Opcode::SUB:
            if (instr.dst < registers.size() && instr.src1 < registers.size() && instr.src2 < registers.size())
                registers[instr.dst] = registers[instr.src1] - registers[instr.src2];
            break;

        case Opcode::CMP:
            if (instr.src1 < registers.size() && instr.src2 < registers.size())
                zero_flag = (registers[instr.src1] == registers[instr.src2]);
            break;

        case Opcode::JMP:
            if (instr.dst < memory.size()) {
                if (instr.src1 == 0 || (instr.src1 == 1 && zero_flag)) {
                    pc = instr.dst;
                }
            }
            break;
    }
}
