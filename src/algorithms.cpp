// algorithm.cpp
#include "algorithms.hpp"

std::vector<Instruction> createFactorialProgram(uint32_t input_addr, uint32_t const_one_addr, uint32_t result_addr) {
    return {
        {Opcode::LOAD, 0, const_one_addr, 0},   // R0 = 1 (result)
        {Opcode::LOAD, 1, input_addr, 0},       // R1 = n
        {Opcode::LOAD, 2, const_one_addr, 0},   // R2 = 1

        // loop_start
        {Opcode::CMP, 0, 1, 2},                 // if R1 == 1
        {Opcode::JMP, 8, 1, 0},                 // if ZF == 1 → skip MUL/SUB

        {Opcode::MUL, 0, 0, 1},                 // result *= i
        {Opcode::SUB, 1, 1, 2},                 // i--
        {Opcode::JMP, 3, 0, 0},                 // jump to loop_start

        // loop_end
        {Opcode::STORE, result_addr, 0, 0},     // RAM[result_addr] = result
        {Opcode::HALT, 0, 0, 0}
    };
}


std::vector<Instruction> createSumListProgram(uint32_t array_addr, uint32_t length_addr, uint32_t result_addr) {
    return {
        {Opcode::LOAD, 0, length_addr, 0},      // R0 = array length (counter)
        {Opcode::LOAD, 1, array_addr, 0},       // R1 = current array element (initially array[0])
        {Opcode::LOAD, 4, length_addr, 0},      // R4 = length (used to calculate address)
        {Opcode::LOAD, 2, result_addr, 0},      // R2 = sum
        {Opcode::LOAD, 3, array_addr, 0},       // R3 = array pointer (starting addr)

        // loop_start @ pc = 5
        {Opcode::CMP, 0, 3, 0},                 // while counter > 0
        {Opcode::JMP, 11, 1, 0},                // if ZF (i.e., counter == 0), exit

        {Opcode::LOAD, 1, 3, 0},                // R1 = RAM[array_ptr]
        {Opcode::ADD, 2, 2, 1},                 // sum += R1
        {Opcode::ADD, 3, 3, 5},                 // array_ptr++ (increment address)
        {Opcode::SUB, 0, 0, 5},                 // counter--

        {Opcode::JMP, 5, 0, 0},                 // jump to loop_start

        {Opcode::STORE, result_addr, 2, 0},     // store result
        {Opcode::HALT, 0, 0, 0}
    };
}