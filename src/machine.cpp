// src/machine.cpp
#include "machine.hpp"
#include <iostream>
#include <cstring>

RiscMachine::RiscMachine(size_t program_size, size_t data_size) {
    program_memory.resize(program_size);
    data_memory.resize(data_size);
}

void RiscMachine::loadProgram(const std::vector<Instruction>& program) {
    program_memory = program;
    pc = 0;
    zero_flag = false;
    registers.fill(0);
}
void RiscMachine::run() {
    while (pc < program_memory.size()) {
        Instruction instr = program_memory[pc];
        pc++;
        execute(instr);
        if (instr.opcode == Opcode::HALT) break;
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
            break;

        case Opcode::LOAD:
            if (instr.dst < registers.size() && instr.src1 < data_memory.size())
                registers[instr.dst] = data_memory[instr.src1];
            break;

        case Opcode::STORE:
            if (instr.dst < data_memory.size() && instr.src1 < registers.size()){
                data_memory[instr.dst] = registers[instr.src1];
            }
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
            if (instr.src1 < registers.size() && instr.src2 < registers.size()) {
                zero_flag = (registers[instr.src1] == registers[instr.src2])? true : false;
            }
            break;

        case Opcode::JMP:
                // If src1 is 0, jump unconditionally
                if (instr.src1 == 0 || (instr.src1 == 1 && zero_flag)) {
                    pc = instr.dst;
                }
            
            break;

        case Opcode::MUL:
            if (instr.dst < registers.size() && instr.src1 < registers.size() && instr.src2 < registers.size()){
                    registers[instr.dst] = registers[instr.src1] * registers[instr.src2];
                }      
            break;

        case Opcode::DIV:
            if (instr.dst < registers.size() && instr.src1 < registers.size() && instr.src2 < registers.size()) {
                if (registers[instr.src2] != 0)
                    registers[instr.dst] = registers[instr.src1] / registers[instr.src2];
                else
                    std::cerr << "Error: Division by zero at PC=" << pc-1 << std::endl;
                    //pc = program_memory.size(); // Halt the program by setting PC out of bounds
            }
            break;
    }
}

void RiscMachine::setMemoryValue(uint32_t address, uint32_t value) {
    if (address < data_memory.size())
        data_memory[address] = value;
}

uint32_t RiscMachine::getMemoryValue(uint32_t address) const {
    if (address < data_memory.size())
        return data_memory[address];
    return 0;
}