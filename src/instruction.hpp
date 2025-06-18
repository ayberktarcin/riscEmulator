/**
 * @file instruction.hpp
 * @brief Defines the Opcode enumeration and Instruction struct for the RISC emulator.
 */

#pragma once

#include <cstdint>

/**
 * @enum Opcode
 * @brief Represents the supported operation codes for the RISC machine.
 */
enum class Opcode {
    HALT,   /**< Stop execution */
    LOAD,   /**< Load value from memory to register */
    STORE,  /**< Store value from register to memory */
    ADD,    /**< Add two data_registers and store result in destination */
    SUB,    /**< Subtract two data_registers and store result in destination */
    CMP,    /**< Compare two data_registers and set flags */
    JMP,    /**< Jump to specified address */
    MUL,    /**< Multiply two data_registers and store result in destination */
    DIV,     /**< Divide two data_registers and store result in destination */
    MOV,    /**< Move value from one register to another */
};

/**
 * @struct Instruction
 * @brief Represents a single instruction for the RISC machine.
 */
struct Instruction {
    Opcode opcode;      /**< The operation code to execute */
    uint32_t dst = 0;   /**< Destination register or address */
    uint32_t src1 = 0;  /**< First source register or value */
    uint32_t src2 = 0;  /**< Second source register or value */
};
