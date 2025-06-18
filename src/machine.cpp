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
    pc  = 0;
    status_register = {};  // Reset status register
    data_registers.fill(0);
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
    status_register = {};  // Reset status register
    data_registers.fill(0);
}

void RiscMachine::execute(const Instruction& instr) {
    switch (instr.opcode) {
        case Opcode::HALT:
            break;

        case Opcode::LOAD:
            if (instr.dst < data_registers.size() && instr.src1 < data_memory.size()){
                data_registers[instr.dst] = data_memory[instr.src1];
                std::cout << "Loading R" << instr.dst << " with value from memory address " 
                          << instr.src1 << " - " << data_memory[instr.src1] << std::endl;
            }
            break;

        case Opcode::STORE:
            if (instr.dst < data_memory.size() && instr.src1 < data_registers.size()){
                data_memory[instr.dst] = data_registers[instr.src1];
            }
            break;

        case Opcode::ADD:
            if (instr.dst < data_registers.size() && instr.src1 < data_registers.size() && instr.src2 < data_registers.size())
                data_registers[instr.dst] = data_registers[instr.src1] + data_registers[instr.src2];
                std::cout << "Adding R" << instr.src1<< " - " << data_registers[instr.src1]  <<" and R" 
                         << instr.src2 << " - " << data_registers[instr.src2] <<" into R" << instr.dst << std::endl;
 
            break;

        case Opcode::SUB:
            if (instr.dst < data_registers.size() && instr.src1 < data_registers.size() && instr.src2 < data_registers.size())
                data_registers[instr.dst] = data_registers[instr.src1] - data_registers[instr.src2];
            break;

        case Opcode::CMP:
            if (instr.src1 < data_registers.size() && instr.src2 < data_registers.size()) {
                status_register.ZF = (data_registers[instr.src1] == data_registers[instr.src2]) ? 1 : 0;
            }else {
                //std::cerr << "Error: Compare instruction with invalid register indices at PC=" << pc-1 << std::endl;
                status_register.ZF = 0; // Reset ZF on error
            }
            break;

        case Opcode::JMP:
            if (instr.dst < program_memory.size()) {
                if (instr.src1 == 0 || (instr.src1 == 1 && status_register.ZF)) {
                    pc = instr.dst;
                    std::cout << "Jumping to address " << instr.dst << " at PC=" << pc-1 << std::endl;
                }
            }else{
                std::cerr << "Error: Jump to out of bounds address at PC=" << pc-1 << std::endl;
                //pc = program_memory.size(); // Halt the program by setting PC out of bounds   
            }
            break;

        case Opcode::MUL:
            if (instr.dst < data_registers.size() && instr.src1 < data_registers.size() && instr.src2 < data_registers.size()){
                    data_registers[instr.dst] = data_registers[instr.src1] * data_registers[instr.src2];
                }      
            break;

        case Opcode::DIV:
            if (instr.dst < data_registers.size() && instr.src1 < data_registers.size() && instr.src2 < data_registers.size()) {
                if (data_registers[instr.src2] != 0) {
                    data_registers[instr.dst] = data_registers[instr.src1] / data_registers[instr.src2];
                    status_register.DF = 0; // Clear division by zero flag
                } else {
                    std::cerr << "Error: Division by zero at PC=" << pc-1 << std::endl;
                    status_register.DF = 1; // Set division by zero flag
                    //pc = program_memory.size(); // Halt the program by setting PC out of bounds
                }
            }
            break;
        
        case Opcode::MOV:
            if (instr.dst < data_registers.size() && instr.src1 < data_registers.size()) {
                data_registers[instr.dst] = data_registers[instr.src1];
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

StatusRegister RiscMachine::getStatusRegister() const {
    return status_register;
}