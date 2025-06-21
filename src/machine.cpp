// src/machine.cpp
#include "machine.hpp"
#include "logging.hpp"
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
            LOG_INFO("HALT instruction encountered. Stopping execution.");
            pc = program_memory.size();  // Halt the program by setting PC out of bounds
            break;

        case Opcode::LOAD:
      // ─────────────────────────────────────────────────────────────────────────────
        // LOAD Instruction: Supports direct, indirect, and immediate value loading
        //
        // - This instruction loads a value into a register (R[dst]).
        //
        // - Three addressing modes are supported via the `src2` field:
        //
        //   • Direct Mode   (src2 == 0): src1 is treated as an immediate RAM address
        //       Example:
        //         {Opcode::LOAD, 4, 100, 0}
        //         → R4 = RAM[100]
        //
        //   • Indirect Mode (src2 == 1): src1 is a register index holding a RAM address
        //       Example:
        //         R0 = 100;  // R0 holds address
        //         {Opcode::LOAD, 4, 0, 1}
        //         → R4 = RAM[R0]
        //
        //   • Immediate Mode (src2 == 2): src1 is treated as a literal value
        //       Example:
        //         {Opcode::LOAD, 4, 42, 2}
        //         → R4 = 42
        //
        // - This supports both traditional memory access and immediate constant assignment.
        // - Enables pointer logic and constant register initialization in programs.
        // ─────────────────────────────────────────────────────────────────────────────
            if (instr.dst < data_registers.size()) {
                uint32_t value = 0;
        
                if (instr.src2 == 0 && instr.src1 < data_memory.size()) {
                    value = data_memory[instr.src1]; // direct mode
                } else if (instr.src2 == 1 && instr.src1 < data_registers.size()) {
                    value = data_memory[data_registers[instr.src1]]; // indirect mode
                } else if (instr.src2 == 2) {
                    // Immediate value
                    value = instr.src1;
                }else {
                    break; // invalid
                }
                data_registers[instr.dst] = value;
                LOG_INFO("Loading R" << instr.dst << " value "<< value);
            }
            break;

        case Opcode::STORE:
            if (instr.dst < data_memory.size() && instr.src1 < data_registers.size()){
                data_memory[instr.dst] = data_registers[instr.src1];
                LOG_INFO("Storing R" << instr.src1 << " value " << data_registers[instr.src1] 
                          << " into RAM[" << instr.dst << "]" );
            }
            break;

        case Opcode::ADD:
            if (instr.dst < data_registers.size() && instr.src1 < data_registers.size() && instr.src2 < data_registers.size())
                data_registers[instr.dst] = data_registers[instr.src1] + data_registers[instr.src2];
                LOG_INFO("Adding R" << instr.src1<< " - " << data_registers[instr.src1]  <<" and R" 
                         << instr.src2 << " - " << data_registers[instr.src2] <<" into R" << instr.dst);
 
            break;

        case Opcode::SUB:
            if (instr.dst < data_registers.size() && instr.src1 < data_registers.size() && instr.src2 < data_registers.size())
                data_registers[instr.dst] = data_registers[instr.src1] - data_registers[instr.src2];
            break;

        case Opcode::CMP:
            if (instr.src1 < data_registers.size() && instr.src2 < data_registers.size()) {
                status_register.ZF = (data_registers[instr.src1] == data_registers[instr.src2]) ? 1 : 0;
            }else {
                LOG_ERROR("Error: Compare instruction with invalid register indices at PC=" << pc-1);
                status_register.ZF = 0; // Reset ZF on error
            }
            break;

        case Opcode::JMP:
            if (instr.dst < program_memory.size()) {
                if (instr.src1 == 0 || (instr.src1 == 1 && status_register.ZF)) {
                    pc = instr.dst;
                    LOG_INFO("Jumping to address " << instr.dst << " at PC=" << pc-1);
                }
            }else{
                LOG_ERROR("Error: Jump to out of bounds address at PC=" << pc-1);
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
                    LOG_ERROR("Error: Division by zero at PC=" << pc-1);
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