/**
 * @file machine.hpp
 * @brief Defines the RiscMachine class and StatusRegister struct for RISC emulation.
 *
 * This file contains the declaration of the RiscMachine class, which provides
 * methods for loading and executing programs, manipulating memory, and accessing
 * the status register in a simulated RISC environment.
 */

#pragma once

#include "instruction.hpp"
#include <array>
#include <vector>
#include <cstdint>

/**
 * @struct StatusRegister
 * @brief Represents the status register flags for the RISC machine.
 *
 * Contains individual flag bits for zero, carry, negative, overflow, and division by zero.
 */
struct StatusRegister {
    /** @brief Zero flag compare (1 if result is zero) */
    uint32_t ZF : 1;
    /** @brief Carry flag (1 if carry occurred) */
    uint32_t CF : 1;
    /** @brief Negative flag (1 if result is negative) */
    uint32_t NF : 1;
    /** @brief Overflow flag (1 if overflow occurred) */
    uint32_t OF : 1;
    /** @brief Division by zero flag (1 if division by zero occurred) */
    uint32_t DF : 1;
    /** @brief Reserved bits (unused) */
    uint32_t reserved : 28;
};

/**
 * @file machine.hpp
 * @brief Defines the RiscMachine class, which emulates a simple RISC architecture.
 *
 * This file contains the declaration of the RiscMachine class, which provides
 * methods for loading and executing programs, manipulating memory, and accessing
 * the status register in a simulated RISC environment.
 */

class RiscMachine {
public:
    /**
     * @brief Constructs a RiscMachine instance.
     * @param program_size The initial size of the program memory (default: 256).
     * @param data_size The initial size of the data memory (default: 1024).
     */
    RiscMachine(size_t program_size = 256, size_t data_size = 1024);

    /**
     * @brief Loads a program into the program memory.
     * @param program A vector of instructions to load.
     */
    void loadProgram(const std::vector<Instruction>& program);

    /**
     * @brief Executes the loaded program from the current program counter.
     */
    void run();

    /**
     * @brief Resets the machine state, including registers and memory.
     */
    void reset();

    /**
     * @brief Sets a value in data memory at the specified address.
     * @param address The memory address to set.
     * @param value The value to write to memory.
     */
    void setMemoryValue(uint32_t address, uint32_t value);

    /**
     * @brief Retrieves a value from data memory at the specified address.
     * @param address The memory address to read.
     * @return The value stored at the given address.
     */
    uint32_t getMemoryValue(uint32_t address) const;

    /**
     * @brief Gets the current status register.
     * @return The current StatusRegister value.
     */
    StatusRegister getStatusRegister() const;

private:
    /**
     * @brief Executes a single instruction.
     * @param instr The instruction to execute.
     */
    void execute(const Instruction& instr);

    std::array<uint32_t, 16> data_registers{};  // R0–R15
    StatusRegister status_register{};

    uint32_t pc = 0;  // program counter

    std::vector<Instruction> program_memory;
    std::vector<uint32_t> data_memory;
};