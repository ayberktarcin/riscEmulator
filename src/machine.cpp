/**
 * @file machine.cpp
 * @brief Implementation of the RISC emulator machine.
 */

#include "machine.hpp"
#include "logging.hpp"
#include <iostream>
#include <cstring>

/**
 * @brief Constructs a RiscMachine with specified program and data memory sizes.
 * 
 * @param program_size The size of the program memory.
 * @param data_size The size of the data memory.
 */
RiscMachine::RiscMachine(size_t program_size, size_t data_size) {
    program_memory.resize(program_size);
    data_memory.resize(data_size);
}

/**
 * @brief Loads a program into the machine's program memory.
 * 
 * @param program A vector of instructions to load into the program memory.
 */
void RiscMachine::loadProgram(const std::vector<Instruction>& program) {
    program_memory = program;
    pc = 0;  // Reset the program counter to the start of the program
    status_register = {};  // Reset the status register
    data_registers.fill(0);  // Clear all data registers
}

/**
 * @brief Executes the loaded program until a HALT instruction is encountered or the program ends.
 */
void RiscMachine::run() {
    while (pc < program_memory.size()) {
        Instruction instr = program_memory[pc];  // Fetch the next instruction
        pc++;  // Increment the program counter
        execute(instr);  // Execute the instruction
        if (instr.opcode == Opcode::HALT) break;  // Stop execution on HALT
    }
}

/**
 * @brief Resets the machine to its initial state.
 * 
 * Resets the program counter, clears the status register, and resets all data registers.
 */
void RiscMachine::reset() {
    pc = 0;  // Reset the program counter
    status_register = {};  // Reset the status register
    data_registers.fill(0);  // Clear all data registers
}

/**
 * @brief Executes a single instruction on the RISC machine.
 * 
 * This function is the core of the RISC machine emulator, responsible for 
 * decoding and executing instructions based on their opcode and operands. 
 * It supports a variety of operations including arithmetic, memory access, 
 * control flow, and flag manipulation. Each instruction modifies the state 
 * of the machine, including registers, memory, and status flags, as needed.
 * 
 * Supported operations include:
 * - HALT: Stops execution.
 * - LOAD/STORE: Memory access operations.
 * - ADD/SUB/MUL/DIV: Arithmetic operations with flag updates.
 * - CMP: Compares two registers and updates the zero flag.
 * - JMP: Conditional and unconditional jumps.
 * - MOV: Copies data between registers.
 * - CHECK_FLAG: Reads specific status flags into a register.
 * 
 * @param instr The instruction to execute, containing the opcode and operands.
 */
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
            if (instr.dst < data_registers.size() &&
                instr.src1 < data_registers.size() &&
                instr.src2 < data_registers.size()) {
        
                uint32_t a = data_registers[instr.src1];
                uint32_t b = data_registers[instr.src2];
                uint64_t result = static_cast<uint64_t>(a) + b;
        
                data_registers[instr.dst] = static_cast<uint32_t>(result);
                
                // Carry flag: result exceeded 32 bits
                status_register.CF = (result > UINT32_MAX);

                // Negative flag: result MSB is 1
                status_register.NF = (result >> 31) & 1;
        
                LOG_INFO("Adding R" << instr.src1 << " - " << data_registers[instr.src1]
                         << " and R" << instr.src2 << " - " << data_registers[instr.src2]
                         << " into R" << instr.dst << (status_register.CF ? " [CARRY]" : ""));
            }
            break;

        case Opcode::SUB:
            if (instr.dst < data_registers.size() &&
            instr.src1 < data_registers.size() &&
            instr.src2 < data_registers.size()) {

                uint32_t lhs = data_registers[instr.src1];
                uint32_t rhs = data_registers[instr.src2];
                uint32_t result = lhs - rhs;

                data_registers[instr.dst] = result;

                // Carry flag (in subtraction, CF = borrow occurred → if lhs < rhs)
                status_register.CF = (lhs < rhs);

                // Negative flag (set if MSB is 1 → interpreted as signed negative)
                status_register.NF = ((result >> 31) & 1);
            }
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
            if (instr.dst < data_registers.size() && 
                instr.src1 < data_registers.size() && 
                instr.src2 < data_registers.size()){
                    uint64_t result = static_cast<uint64_t>(data_registers[instr.src1]) * data_registers[instr.src2];
                    data_registers[instr.dst] = static_cast<uint32_t>(result);

                    // Set overflow flag if result doesn't fit in 32 bits
                    status_register.OF = (result > UINT32_MAX);
                    status_register.NF = (result >> 31) & 1;

                    LOG_INFO( "MUL: R" << instr.dst << " = " << data_registers[instr.src1]
                    << " * " << data_registers[instr.src2]
                    << " = " << result
                    << (status_register.OF ? " [OVERFLOW]" : ""));
                }      
            break;

        case Opcode::DIV:
            if (instr.dst < data_registers.size() && 
                instr.src1 < data_registers.size() && 
                instr.src2 < data_registers.size()) {

                    uint32_t dividend = data_registers[instr.src1];
                    uint32_t divisor = data_registers[instr.src2];
                    // Check for division by zero                    
                    if (divisor== 0) {
                        LOG_ERROR("Error: Division by zero at PC=" << pc-1);
                        status_register.DF = 1; // Set division by zero flag
                        //pc = program_memory.size(); // Halt the program by setting PC out of bounds
                    } else {
                        uint32_t result = dividend / divisor;
                        data_registers[instr.dst] = result;

                        status_register.NF = (result >> 31) & 1;
                        status_register.DF = 0;
                    }
                    // No meaningful overflow or carry for unsigned div
                    status_register.OF = 0;
                    status_register.CF = 0;
            }
            break;
        
        case Opcode::MOV:
            if (instr.dst < data_registers.size() && instr.src1 < data_registers.size()) {
                data_registers[instr.dst] = data_registers[instr.src1];
            }
            break;
        
        case Opcode::CHECK_FLAG:
        if (instr.dst < data_registers.size()) {
            uint32_t value = 0;
            switch (instr.src1) {
                case 0: value = status_register.ZF; break; // Zero Flag
                case 1: value = status_register.CF; break; // Carry Flag
                case 2: value = status_register.NF; break; // Negative Flag
                case 3: value = status_register.OF; break; // Overflow Flag
                case 4: value = status_register.DF; break; // Division by Zero Flag
                default: value = 0; break;  // unknown flag
            }
            data_registers[instr.dst] = value;
    
            LOG_INFO("CHECK_FLAG: R" << instr.dst << " = flag[" << instr.src1 << "] = " << value);
        }
        break;
    }
}
/**
 * @brief Sets a value in the data memory at the specified address.
 * 
 * @param address The address in the data memory to set the value.
 * @param value The value to set at the specified address.
 */
void RiscMachine::setMemoryValue(uint32_t address, uint32_t value) {
    if (address < data_memory.size())
        data_memory[address] = value;
}

/**
 * @brief Retrieves a value from the data memory at the specified address.
 * 
 * @param address The address in the data memory to retrieve the value from.
 * @return The value at the specified address, or 0 if the address is out of bounds.
 */
uint32_t RiscMachine::getMemoryValue(uint32_t address) const {
    if (address < data_memory.size())
        return data_memory[address];
    return 0;
}

/**
 * @brief Retrieves the current status register of the machine.
 * 
 * @return The current status register containing flags such as ZF, CF, NF, OF, and DF.
 */
StatusRegister RiscMachine::getStatusRegister() const {
    return status_register;
}